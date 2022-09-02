#include "bliss_app.hpp"

#include <sstream>

#include "build_info.hpp"  //(CosmicBagel) Generated by cmake using buildInfo.h.in template
#include "components.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "entity_memory_pool.hpp"
#include "log_router.hpp"
#include "perf_timer.hpp"

typedef struct Matrix2d {
    float a, b;  // top row
    float c, d;  // bottom row
} Matrix2d;

inline Vector2 apply_transformation(Matrix2d t_mat, Vector2 vec) {
    return Vector2{t_mat.a * vec.x + t_mat.b * vec.y,
                   t_mat.c * vec.x + t_mat.d * vec.y};
}

// (CosmicBagel) 2d rotation matrix
// cos theta, sin theta
// -sin theta, cos theta
inline Matrix2d create_rotation_mat(float rads) {
    return Matrix2d{cosf(rads), sinf(rads), -sinf(rads), cosf(rads)};
}

inline bool check_circle_overlap(Vector2 pos_a, Vector2 pos_b, float radius_a,
                                 float radius_b) {
    auto pos_diff = Vector2{pos_b.x - pos_a.x, pos_b.y - pos_a.y};
    auto squared_distance = pos_diff.x * pos_diff.x + pos_diff.y * pos_diff.y;
    auto radius_sum = radius_a + radius_b;
    auto squared_radius = radius_sum * radius_sum;
    return squared_distance < squared_radius;
}

Entity Bliss_App::create_player() {
    const Vector2 initial_pos = {200, 200};
    const Vector2 initial_vel = {0, 0};
    const float collision_radius = 50;

    auto& man = Entity_Manager::instance();
    auto entity = man.add_entity("player");

    entity.add_component<C_Player>();

    auto& pos = entity.add_component<C_Position>();
    pos = initial_pos;

    auto& vel = entity.add_component<C_Velocity>();
    vel = initial_vel;

    auto& bounding_circle = entity.add_component<C_Bounding_Circle>();
    bounding_circle.radius = collision_radius;

    auto& player_tex = entity.add_component<C_Texture>();
    player_tex.texture = santa_sm_tex;

    return entity;
}

void Bliss_App::create_enemy(size_t count, Vector2 no_spawn_center) {
    static std::string enemy_tag = "enemy";
    const Vector2 initial_vel = {0, 0};
    const float collision_radius = 50.0F;
    const float no_spawn_radius = 100.0F;

    auto& e_man = Entity_Manager::instance();
    for (int i = 0; i < count; i++) {
        auto entity = e_man.add_entity(enemy_tag);

        entity.add_component<C_Enemy>();

        // (CosmicBagel) random pos within screen size
        auto rnd_vec =
            Vector2{static_cast<float>(GetRandomValue(0, GetScreenWidth())),
                    static_cast<float>(GetRandomValue(0, GetScreenHeight()))};

        // (CosmicBagel) A is the resulting vector of difference of two
        // positions
        auto diff = Vector2{rnd_vec.x - no_spawn_center.x,
                            rnd_vec.y - no_spawn_center.y};
        auto squared_magintude = diff.x * diff.x + diff.y * diff.y;
        auto sum_radius = collision_radius + no_spawn_radius;
        auto squared_radius = sum_radius * sum_radius;

        // (CosmicBagel) check if the enemy overlaps with the no spawn radius
        if (squared_magintude < squared_radius) {
            auto magnitude = sqrtf(squared_magintude);
            auto A_normalized = Vector2{diff.x / magnitude, diff.y / magnitude};
            rnd_vec = Vector2{A_normalized.x * sum_radius + no_spawn_center.x,
                              A_normalized.y * sum_radius + no_spawn_center.y};
        }

        auto& pos = entity.add_component<C_Position>();
        pos.x = rnd_vec.x;
        pos.y = rnd_vec.y;

        auto& vel = entity.add_component<C_Velocity>();
        vel = initial_vel;

        auto& bounding_circle = entity.add_component<C_Bounding_Circle>();
        bounding_circle.radius = collision_radius;

        auto& tex = entity.add_component<C_Texture>();
        tex.texture = popper_sm_tex;
    }
}

void Bliss_App::create_bullet(C_Position initial_pos, C_Velocity initial_vel,
                              float initial_rot) {
    const float collision_radius = 5.0F;

    static std::string bullet_tag = "bullet";

    auto& man = Entity_Manager::instance();
    auto entity = man.add_entity(bullet_tag);

    entity.add_component<C_PlayerBullet>();

    auto& pos = entity.add_component<C_Position>();
    // (CosmicBagel) can't copy the component whole, as the `active` bool will
    // be overwritten
    pos.x = initial_pos.x;
    pos.y = initial_pos.y;

    auto& vel = entity.add_component<C_Velocity>();
    // (CosmicBagel) can't copy the component whole, as the `active` bool will
    // be overwritten
    vel.x = initial_vel.x;
    vel.y = initial_vel.y;

    auto& bounding_circle = entity.add_component<C_Bounding_Circle>();
    bounding_circle.radius = collision_radius;

    auto& tex = entity.add_component<C_Texture>();
    tex.texture = snowball_tex;

    auto& rot = entity.add_component<C_Rotation>();
    rot.rotation = initial_rot;
}

Bliss_App::Bliss_App()
    : dev_ui(Dev_UI::instance()),
      santa_tex({}),
      santa_cropped_tex({}),
      santa_sm_tex({}),
      snowball_tex({}),
      font_roboto_mono({}),
      font_roboto_mono_sm({}),
      font({}),
      popper_sm_tex({}) {
    log_router_enable();
}

void Bliss_App::run() {
    const int initial_enemy_count = 5;

    TraceLog(LOG_INFO, "Starting " BLISS_FULL_HEADER "...\n");
    InitWindow(screen_width, screen_height, BLISS_FULL_HEADER);

    dev_ui.init();

    TraceLog(LOG_INFO, "Loading fonts");
    load_fonts();

    TraceLog(LOG_INFO, "Loading textures");
    load_textures();
    // (CosmicBagel) raylib set fps
    // SetTargetFPS(144);

    TraceLog(LOG_INFO, "Initializing sim");
    Entity_Manager& man = Entity_Manager::instance();

    auto player = create_player();
    auto& p_pos = player.get_component<C_Position>();
    create_enemy(initial_enemy_count, static_cast<Vector2>(p_pos));
    man.update_manager();

    TraceLog(LOG_INFO, "Starting sim loop");
    while (!input_state.exit_window) {
        handle_input();
        // (CosmicBagel) step sim once
        // if (IsKeyPressed(KEY_EQUAL)|| IsKeyDown(KEY_SPACE))
        { simulation_step(); }
        draw_scene();
        draw_game_ui();
        draw_dev_ui();

        render_dev_ui();
        render_scene();

        man.update_manager();

        // (CosmicBagel) Update metrics plot
        dev_ui.update_plot_axis();
    }

    CloseWindow();
}

void Bliss_App::handle_input() {
    // (CosmicBagel) Note: Input is actually polled inside raylib's EndDrawing
    // (after frame waiting is finished!) so effectively processing input here
    // (at the start of the frame) is as good as processing right after polling
    // for input.

    Perf_Timer function_perf_timer(Metrics::instance().input_time);
    // Entity_Manager& man = Entity_Manager::instance();

    input_state.exit_window = WindowShouldClose();

    input_state.up = IsKeyDown(KEY_W);
    input_state.down = IsKeyDown(KEY_S);
    input_state.left = IsKeyDown(KEY_A);
    input_state.right = IsKeyDown(KEY_D);

    input_state.target_pos = GetMousePosition();
    input_state.fire = IsMouseButtonDown(0);
    input_state.alt_fire = IsMouseButtonPressed(1);

    if (IsKeyPressed(KEY_E)) {
        TraceLog(LOG_INFO, "hi");
    }
}

inline bool check_for_overlap(Entity entity_a, Entity entity_b) {
    auto radius_a = entity_a.get_component<C_Bounding_Circle>().radius;
    auto radius_b = entity_b.get_component<C_Bounding_Circle>().radius;

    auto center_a = static_cast<Vector2>(entity_a.get_component<C_Position>());
    auto center_b = static_cast<Vector2>(entity_b.get_component<C_Position>());

    return check_circle_overlap(center_a, center_b, radius_a, radius_b);
}

void Bliss_App::simulation_step() {
    Perf_Timer function_perf_timer(Metrics::instance().simulation_time);

    // (CosmicBagel) for systems
    // separate loops for separate components, we want to access components
    // across their respective arrays (sequentially in memory), not hopping
    // between different memory regions.

    update_players();
    update_velocity();
    update_enemy_player_collisions();
    update_enemy_bullet_collisions();
    enforce_bullet_boundary();
}

void Bliss_App::update_players() {
    // (CosmicBagel) update player velocity based on input
    auto& e_man = Entity_Manager::instance();
    auto& e_player = e_man.get_entities("player");
    for (Entity player : e_player) {
        const float move_velocity = 400;

        auto& p_vel = player.get_component<C_Velocity>();

        p_vel.y = 0;
        p_vel.x = 0;

        if (input_state.up) {
            p_vel.y -= 1.0;
        }

        if (input_state.down) {
            p_vel.y += 1.0;
        }

        if (input_state.left) {
            p_vel.x -= 1.0;
        }

        if (input_state.right) {
            p_vel.x += 1.0;
        }

        p_vel.y *= move_velocity;
        p_vel.x *= move_velocity;

        if (input_state.fire || input_state.alt_fire) {
            const float bullet_velocity = 200;

            auto& pos = player.get_component<C_Position>();

            // (CosmicBagel) determine velocity vector
            float distance_x = input_state.target_pos.x - pos.x;
            float distance_y = input_state.target_pos.y - pos.y;

            float squared_magnitude =
                distance_x * distance_x + distance_y * distance_y;
            float magnitude = sqrtf(squared_magnitude);

            float vector_x = distance_x / magnitude;
            float vector_y = distance_y / magnitude;

            Vector2 vel{bullet_velocity * vector_x, bullet_velocity * vector_y};

            // (CosmicBagel) determine rotation using the distance vector
            // note our y distance is screen space (unit_y is pointing down)
            // so we need to flip it for standard coordinates
            float rads = atan2(-distance_y, distance_x);

            C_Velocity c_vel{vel};
            create_bullet(pos, c_vel, rads);
        }
    }
}

void Bliss_App::update_velocity() {
    auto& e_man = Entity_Manager::instance();
    auto& entities = e_man.get_entities();

    // (CosmicBagel) update position based on velocity
    for (Entity entity : entities) {
        if (entity.has_component<C_Velocity>() &&
            entity.has_component<C_Position>()) {
            auto& pos = entity.get_component<C_Position>();
            auto& vel = entity.get_component<C_Velocity>();
            pos.x += vel.x * GetFrameTime();
            pos.y += vel.y * GetFrameTime();
        }
    }
}
void Bliss_App::update_enemy_player_collisions() {
    auto& e_man = Entity_Manager::instance();
    auto& e_player = e_man.get_entities("player");
    auto& e_enemies = e_man.get_entities("enemy");

    for (Entity player : e_player) {
        for (Entity enemy : e_enemies) {
            if (player.is_active() && enemy.is_active() &&
                check_for_overlap(player, enemy)) {
                // (CosmicBagel) player dies
                // TraceLog(LOG_INFO, "Player overlap with enemy");
                auto& p_pos = player.get_component<C_Position>();
                e_man.remove_entity(player);
                if (lives > 0) {
                    lives--;
                    auto new_p = create_player();
                    p_pos = new_p.get_component<C_Position>();
                }

                size_t enemy_count = e_enemies.size();

                // (CosmicBagel) clear all enemies
                for (Entity enemy_removing : e_enemies) {
                    e_man.remove_entity(enemy_removing);
                }
                create_enemy(enemy_count, static_cast<Vector2>(p_pos));
                break;
            }
        }
    }
}

void Bliss_App::update_enemy_bullet_collisions() {
    auto& e_man = Entity_Manager::instance();
    auto& e_enemies = e_man.get_entities("enemy");
    auto& e_bullets = e_man.get_entities("bullet");

    for (Entity enemy : e_enemies) {
        for (Entity bullet : e_bullets) {
            // (CosmicBagel) enemies and bullets are not removed from the list
            // immediately they are removed on update, however the is_active
            // flag is immediately updated so we can read that to see if this
            // overlap is real
            if (enemy.is_active() && bullet.is_active() &&
                check_for_overlap(enemy, bullet)) {
                // enemy dies
                e_man.remove_entity(bullet);
                e_man.remove_entity(enemy);

                const float half_divisor = 2.0F;
                // (CosmicBagel) determine default position (screen center) if
                // player doesn't exist
                Vector2 pos{
                    static_cast<float>(GetScreenWidth()) / half_divisor,
                    static_cast<float>(GetScreenHeight()) / half_divisor};

                auto p_entities = e_man.get_entities("player");
                if (!p_entities.empty()) {
                    auto first_player = p_entities[0];
                    auto& p_pos = first_player.get_component<C_Position>();
                    pos = static_cast<Vector2>(p_pos);
                }

                // pos is used as center to NOT spawn at (no spawn zone)
                create_enemy(1, pos);
            }
        }
    }
}

void Bliss_App::enforce_bullet_boundary() {
    auto& e_man = Entity_Manager::instance();
    auto& e_bullets = e_man.get_entities("bullet");

    for (Entity bullet : e_bullets) {
        const float boundary_square_size = 1000.0F;

        // (CosmicBagel) clean up out of bounds bullets
        auto& pos = bullet.get_component<C_Position>();
        if (pos.x < -boundary_square_size || pos.x > boundary_square_size ||
            pos.y < -boundary_square_size || pos.y > boundary_square_size) {
            e_man.remove_entity(bullet);
        }
    }
}

void Bliss_App::draw_scene() {
    Perf_Timer function_perf_timer(Metrics::instance().draw_prep_time);
    BeginDrawing();

    Entity_Manager& man = Entity_Manager::instance();

    ClearBackground(WHITE);

    // (CosmicBagel) DrawTexture(santa_tex,
    //	screen_width / 2 - santa_tex.width / 2,
    //	screen_height / 2 - santa_tex.height / 2, WHITE);

    // draw
    for (Entity entity : man.get_entities()) {
        bool hasPos = entity.has_component<C_Position>();
        if (hasPos && entity.has_component<C_Texture>()) {
            C_Position& pos = entity.get_component<C_Position>();
            C_Texture& tex = entity.get_component<C_Texture>();
            if (entity.has_component<C_Rotation>()) {
                C_Rotation& rot = entity.get_component<C_Rotation>();
                // (CosmicBagele) note in case I want to let something rotate or
                // scale (I suspect the above function is faster tho)

                Matrix2d rot_mat = create_rotation_mat(rot.rotation);
                // (CosmicBagel) top left corner relative to the center of the
                // texture to be consistent with the pixel space, the unit
                // vectors point down and right
                Vector2 top_left{static_cast<float>(tex.texture.width) / -2,
                                 static_cast<float>(tex.texture.height) / -2};
                Vector2 rotated_corner =
                    apply_transformation(rot_mat, top_left);

                Vector2 renderPos{pos.x + rotated_corner.x,
                                  pos.y + rotated_corner.y};

                // (CosmicBagel) raylib's degrees are inverted for some reason
                const float DEGREES = 360;
                const float scale = 1.0F;
                DrawTextureEx(tex.texture, renderPos,
                              DEGREES - rot.rotation * RAD2DEG, scale, WHITE);
                // DrawRectangle((int)renderPos.x, (int)renderPos.y, 3, 3,
                // BLUE);
                DrawCircle(static_cast<int>(renderPos.x),
                           static_cast<int>(renderPos.y), 1.0F, BLUE);
            } else {
                DrawTexture(tex.texture,
                            static_cast<int>(pos.x) - tex.texture.width / 2,
                            static_cast<int>(pos.y) - tex.texture.height / 2,
                            WHITE);
                DrawCircle(static_cast<int>(pos.x) - tex.texture.width / 2,
                           static_cast<int>(pos.y) - tex.texture.height / 2, 1,
                           BLUE);
                // DrawRectangle((int)pos.x - tex.texture.width / 2, (int)pos.y
                // - tex.texture.height / 2, 3, 3, BLUE);
            }
        }

        // (CosmicBagel) bounding circle debug
        if (hasPos && entity.has_component<C_Bounding_Circle>()) {
            C_Position& pos = entity.get_component<C_Position>();
            C_Bounding_Circle& bounds =
                entity.get_component<C_Bounding_Circle>();
            Color translucentRed = RED;
            const int SEMI_TRANSPARENT_ALPHA = 128;
            translucentRed.a = SEMI_TRANSPARENT_ALPHA;
            DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y),
                            bounds.radius, RED);
            DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y),
                       bounds.radius, translucentRed);
        }

        if (hasPos) {
            C_Position& pos = entity.get_component<C_Position>();
            DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 1.0F,
                       GREEN);
            // DrawRectangle((int)pos.x, (int)pos.y, 3, 3, GREEN);
        }
    }

    Vector2 text_dim =
        MeasureTextEx(font_roboto_mono, BLISS_FULL_HEADER,
                      static_cast<float>(font_roboto_mono.baseSize), 0);
    Vector2 text_pos;
    const float HALF_DIVISOR = 2.0F;
    text_pos.x = (static_cast<float>(screen_width) - text_dim.x) / HALF_DIVISOR;
    text_pos.y =
        (static_cast<float>(screen_height) - text_dim.y) / HALF_DIVISOR;

    // text_pos.x = ((float)screen_width  - text_dim.x) / 2.0f;
    // text_pos.y = ((float)screen_height - text_dim.y) / 2.0f;

    DrawTextEx(font_roboto_mono, BLISS_FULL_HEADER, text_pos,
               static_cast<float>(font_roboto_mono.baseSize), 0, DARKGRAY);
}

void Bliss_App::draw_game_ui() {
    Perf_Timer function_perf_timer(Metrics::instance().game_ui_time);
    std::ostringstream lives_display_string;

    lives_display_string << "Lives: " << lives;

    const Vector2 text_pos{40, 40};

    const Vector2 text_bg_pos{30, 30};
    const Vector2 text_bg_dim{120, 50};
    DrawRectangle(static_cast<int>(text_bg_pos.x),
                  static_cast<int>(text_bg_pos.y),
                  static_cast<int>(text_bg_dim.x),
                  static_cast<int>(text_bg_dim.y), BLACK);
    DrawTextEx(font_roboto_mono, lives_display_string.str().c_str(), text_pos,
               static_cast<float>(font_roboto_mono.baseSize), 0, WHITE);
}

void Bliss_App::draw_dev_ui() {
    static size_t current_entity = 0;
    //----------------------------------------------------------------------------------
    // Dev UI
    Perf_Timer function_perf_timer(Metrics::instance().dev_ui_time);

    Dev_UI::new_frame();
    dev_ui.draw();

    ImVec2 vec2_zero = {0, 0};
    ImVec2 vec2_one = {1.0F, 1.0F};
    ImVec4 vec4_zero = {0, 0, 0, 0};
    ImVec4 vec4_one = {1.0F, 1.0F, 1.0F, 1.0F};
    ImVec2 size;
    const float quarter_divisor = 4.0F;
    size.x = static_cast<float>(santa_tex.width) / quarter_divisor;
    size.y = static_cast<float>(santa_tex.height) / quarter_divisor;

    if (ImGui::Begin("Entity viewer", nullptr, 0)) {
        // ImGui::InputInt("current_entity", &current_entity);
        const size_t step = 1;
        const size_t fast_step = 10;
        ImGui::InputScalar("", ImGuiDataType_U64, &current_entity, &step,
                           &fast_step);

        auto& man = Entity_Manager::instance();
        auto& entities = man.get_entities();
        for (auto entity : entities) {
            if (entity.get_id() == current_entity) {
                ImGui::Text("Tag: %s", entity.get_tag().c_str());
                if (entity.has_component<C_Position>()) {
                    auto& pos = entity.get_component<C_Position>();
                    ImGui::Text("Pos X: %f", pos.x);
                    ImGui::Text("Pos Y: %f", pos.y);
                }
                break;
            }
        }
    }
    ImGui::End();

    // ImGui::ShowDemoWindow();

    log_gui.draw("Log");
}

void Bliss_App::render_dev_ui() {
    Perf_Timer function_perf_timer(Metrics::instance().dev_ui_render_time);
    Dev_UI::render();
}

void Bliss_App::render_scene() {
    Perf_Timer function_perf_timer(Metrics::instance().drawing_time);
    EndDrawing();
}

void Bliss_App::load_fonts() {
    const int regular_size = 28;
    const int regular_small_size = 14;
    const int small_size = 12;
    const int roboto_mono_glyph_count = 255;
    const int merriweather_glyph_count = 255;

    font_roboto_mono =
        LoadFontEx("data/fonts/RobotoMono-Regular.ttf", regular_size, nullptr,
                   roboto_mono_glyph_count);

    font_roboto_mono_sm =
        LoadFontEx("data/fonts/RobotoMono-Regular.ttf", small_size, nullptr,
                   roboto_mono_glyph_count);

    font = LoadFontEx("data/fonts/Merriweather-Regular.ttf", regular_small_size,
                      nullptr, merriweather_glyph_count);
}

void Bliss_App::load_textures() {
    santa_tex = LoadTexture("data/santa/Idle (1).png");
    santa_sm_tex = LoadTexture("data/santa/Idle (1) - Cropped - Small.png");
    popper_sm_tex =
        LoadTexture("data/santa/Idle (1) - Cropped - Small - Inverted.png");
    santa_cropped_tex = LoadTexture("data/santa/Idle (1) - Cropped.png");
    snowball_tex = LoadTexture("data/snowball/snowball_01_sm_mirrored.png");
}

Bliss_App::~Bliss_App() {
    TraceLog(LOG_INFO, "Unloading fonts...");
    UnloadFont(font_roboto_mono);
    UnloadFont(font_roboto_mono_sm);
    UnloadFont(font);

    TraceLog(LOG_INFO, "Unloading textures...");
    UnloadTexture(santa_tex);
    UnloadTexture(santa_sm_tex);
    UnloadTexture(popper_sm_tex);
    UnloadTexture(santa_cropped_tex);
    UnloadTexture(snowball_tex);
}
