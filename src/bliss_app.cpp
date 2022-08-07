#include "build_info.h" //Generated by cmake using buildInfo.h.in template

#include "bliss_app.h"
#include "log_router.h"
#include "perf_timer.h"
#include "entity_manager.h"
#include "entity_memory_pool.h"
#include "entity.h"
#include "components.h"

#include <sstream>

void Bliss_App::create_player()
{
	auto& man = Entity_Manager::instance();
	auto e = man.add_entity("player");

	e.add_component<C_Player>();

	auto& initial_pos = e.add_component<C_Position>();
	initial_pos.x = 200;
	initial_pos.y = 200;

	auto& inital_vel = e.add_component<C_Velocity>();
	inital_vel.x = 0;
	inital_vel.y = 0;

	auto& bounding_circle = e.add_component<C_Bounding_Circle>();
	bounding_circle.radius = 50;

	auto& player_tex = e.add_component<C_Texture>();
	player_tex.texture = santa_sm_tex;
}

void Bliss_App::create_enemy(int count)
{
	auto& man = Entity_Manager::instance();
	for (int i = 0; i < count; i++) {
		// std::ostringstream s;
		// s << "popper_" << i;
		auto e = man.add_entity("enemy");

		e.add_component<C_Enemy>();

		// random pos within screen size
		int x = GetRandomValue(0, GetScreenWidth());
		int y = GetRandomValue(0, GetScreenHeight());
		TraceLog(LOG_INFO, "popper %d, created at %d, %d", i, x, y);

		auto& pos = e.add_component<C_Position>();
		pos.x = (float)x;
		pos.y = (float)y;

		auto& vel = e.add_component<C_Velocity>();
		vel.x = 0;
		vel.y = 0;

		auto& bounding_circle = e.add_component<C_Bounding_Circle>();
		bounding_circle.radius = 50;

		auto& tex = e.add_component<C_Texture>();
		tex.texture = popper_sm_tex;
	}
}

void Bliss_App::create_bullet(float pos_x, float pos_y, float vel_x, float vel_y)
{
	auto& man = Entity_Manager::instance();
	auto e = man.add_entity("bullet");

	e.add_component<C_PlayerBullet>();

	auto& pos = e.add_component<C_Position>();
	pos.x = (float)pos_x;
	pos.y = (float)pos_y;

	auto& vel = e.add_component<C_Velocity>();
	vel.x = vel_x;
	vel.y = vel_y;

	auto& bounding_circle = e.add_component<C_Bounding_Circle>();
	bounding_circle.radius = 5;

	auto& tex = e.add_component<C_Texture>();
	tex.texture = snowball_tex;
}

Bliss_App::Bliss_App() : dev_ui(Dev_UI::instance())
{
	log_router_enable();
}

void Bliss_App::run()
{
	TraceLog(LOG_INFO, "Starting " BLISS_FULL_HEADER "...\n");
	InitWindow(screen_width, screen_height, BLISS_FULL_HEADER);

	dev_ui.init();

	TraceLog(LOG_INFO, "Loading fonts");
	load_fonts();

	TraceLog(LOG_INFO, "Loading textures");
	load_textures();

	// raylib set fps
	SetTargetFPS(144);

	TraceLog(LOG_INFO, "Initializing sim");
	Entity_Manager& man = Entity_Manager::instance();
	
	create_player();
	create_enemy();
	man.update_manager();

	TraceLog(LOG_INFO, "Starting sim loop");
	while (!input_state.exit_window)
	{
		handle_input();
		simulation_step();
		draw_scene();
		draw_dev_ui();

		render_dev_ui();
		render_scene();

		man.update_manager();

		//Update metrics plot
		dev_ui.plot.UpdateAxes();
	}

	CloseWindow();
}

void Bliss_App::handle_input()
{
	//Input handling
	//----------------------------------------------------------------------------------
	//Note: Input is actually polled inside raylib's EndDrawing (after frame
	//      waiting is finished!) so effectively processing input here (at the 
	//      start of the frame) is as good as processing right after polling
	//      for input.
	Perf_Timer t(dev_ui.metrics.input_time);
	// Entity_Manager& man = Entity_Manager::instance();

	input_state.exit_window = WindowShouldClose();

	input_state.up = IsKeyDown(KEY_W);
	input_state.down = IsKeyDown(KEY_S);
	input_state.left = IsKeyDown(KEY_A);
	input_state.right = IsKeyDown(KEY_D);

	input_state.target_pos = GetMousePosition();
	input_state.fire = IsMouseButtonDown(0);

	if (IsKeyPressed(KEY_E)) {
		TraceLog(LOG_INFO, "hi");
	}

	if (!dev_ui.ig_io->WantCaptureMouse)
	{
		//only take mouse input in the game if imgui isn't catching it
		//clicks and input shouldn't go through a window
	}
	if (!dev_ui.ig_io->WantCaptureKeyboard)
	{
		//check keyboard inputs
	}
	if (!dev_ui.ig_io->WantTextInput)
	{
		//for on mobile & consoles
	}
}

bool check_for_overlap(Entity e1, Entity e2)
{
	auto r1 = e1.get_component<C_Bounding_Circle>().radius;
	auto r2 = e2.get_component<C_Bounding_Circle>().radius;

	auto p1 = e1.get_component<C_Position>();
	auto p2 = e2.get_component<C_Position>();

	auto squared_distance = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
	auto squared_radius = (r1 + r2) * (r1 + r2);

	return squared_distance < squared_radius;
}

void Bliss_App::simulation_step()
{
	//Simulation
	//----------------------------------------------------------------------------------
	Perf_Timer t(dev_ui.metrics.simulation_time);
	// simulate go here

	auto& e_man = Entity_Manager::instance();
	auto& entities = e_man.get_entities();

	// for logic on specific "kinds" of entities
	auto& e_player = e_man.get_entities("player");
	auto& e_enemies = e_man.get_entities("enemy");
	auto& e_bullets = e_man.get_entities("bullet");

	// update player velocity based on input
	for (Entity p : e_player) {
		auto& v = p.get_component<C_Velocity>();

		v.y = 0;
		v.x = 0;

		if (input_state.up) {
			v.y -= 1.0;
		}

		if (input_state.down) {
			v.y += 1.0;
		}

		if (input_state.left) {
			v.x -= 1.0;
		}

		if (input_state.right) {
			v.x += 1.0;
		}

		v.y *= 4;
		v.x *= 4;

		if (input_state.fire) {
			auto& pos = p.get_component<C_Position>();

			float distance_x = input_state.target_pos.x - pos.x;
			float distance_y = input_state.target_pos.y - pos.y;

			float squared_magnitude = distance_x * distance_x + distance_y * distance_y;
			float magnitude = sqrtf(squared_magnitude);

			float vector_x = distance_x / magnitude;
			float vector_y = distance_y / magnitude;

			float vel_x = 12 * vector_x;
			float vel_y = 12 * vector_y;

			create_bullet(pos.x, pos.y, vel_x, vel_y);
		}
	}

	// for systems
	//separate loops for separate components, we want to access components
	//across their respective arrays (sequentially in memory), not hopping
	//between different memory regions.

	// update position based on velocity
	for (Entity e : entities)
	{
		if (e.has_component<C_Velocity>() && e.has_component<C_Position>())
		{
			auto& pos = e.get_component<C_Position>();
			auto& vel = e.get_component<C_Velocity>();
			pos.x += vel.x;
			pos.y += vel.y;
		}
	}

	for (Entity p : e_player) {
		for (Entity e : e_enemies) {
			if (check_for_overlap(p, e)) {
				// player dies
				TraceLog(LOG_INFO, "Player overlap with enemy");
			}
		}
	}

	for (Entity e : e_enemies) {
		for (Entity b : e_bullets) {
			if (check_for_overlap(e, b)) {
				// enemy dies
			}
		}
	}

	for (Entity e : e_bullets) {
		// clean up out of bounds bullets
		auto& pos = e.get_component<C_Position>();
		if (pos.x < -1000 || pos.x > 1000 || pos.y < -1000 || pos.y > 1000) {
			e_man.remove_entity(e);
		}
	}
}

void Bliss_App::draw_scene()
{
	// Draw prep
	//----------------------------------------------------------------------------------
	Perf_Timer t(dev_ui.metrics.draw_prep_time);
	BeginDrawing();

	Entity_Manager& man = Entity_Manager::instance();

	ClearBackground(WHITE);

	//DrawTexture(santa_tex,
	//	screen_width / 2 - santa_tex.width / 2,
	//	screen_height / 2 - santa_tex.height / 2, WHITE);

	// draw
	for (Entity e : man.get_entities())
	{
		bool hasPos = e.has_component<C_Position>();
		if (hasPos && e.has_component<C_Texture>())
		{
			C_Position& pos = e.get_component<C_Position>();
			C_Texture& tex = e.get_component<C_Texture>();
			DrawTexture(tex.texture, (int)pos.x - tex.texture.width / 2, (int)pos.y - tex.texture.height / 2, WHITE);
			DrawCircle((int)pos.x - tex.texture.width / 2, (int)pos.y - tex.texture.height / 2, 1, BLUE);
			//note in case I want to let something rotate or scale (I suspect the above function
			//is faster tho)
			//DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);  
		}

		// bounding circle debug
		if (hasPos && e.has_component<C_Bounding_Circle>())
		{
			C_Position& pos = e.get_component<C_Position>();
			C_Bounding_Circle& bounds = e.get_component<C_Bounding_Circle>();
			Color translucentRed = RED;
			translucentRed.a = 128;
			DrawCircleLines((int)pos.x, (int)pos.y, bounds.radius, RED);
			DrawCircle((int)pos.x, (int)pos.y, bounds.radius, translucentRed);
		}

		if (hasPos)
		{
			C_Position& pos = e.get_component<C_Position>();
			DrawCircle((int)pos.x, (int)pos.y, 1, GREEN);
		}
	}

	Vector2 text_dim = MeasureTextEx(font_roboto_mono, BLISS_FULL_HEADER,
		(float)font_roboto_mono.baseSize, 0);
	Vector2 text_pos;
	text_pos.x = ((float)screen_width - text_dim.x) / 2.0f;
	text_pos.y = ((float)screen_height - text_dim.y) / 2.0f;
	
	//text_pos.x = ((float)screen_width  - text_dim.x) / 2.0f;
	//text_pos.y = ((float)screen_height - text_dim.y) / 2.0f;

	DrawTextEx(font_roboto_mono, BLISS_FULL_HEADER, text_pos,
		(float)font_roboto_mono.baseSize, 0, DARKGRAY);
}

void Bliss_App::draw_dev_ui()
{
	static size_t current_entity = 0;
	//----------------------------------------------------------------------------------
	//Dev UI
	Perf_Timer t(dev_ui.metrics.dev_ui_time);

	dev_ui.new_frame();
	dev_ui.draw();

	ImVec2 vec2_zero = { 0.0f, 0.0f };
	ImVec2 vec2_one = { 1.0f, 1.0f };
	ImVec4 vec4_zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	ImVec4 vec4_one = { 1.0f, 1.0f, 1.0f, 1.0f };
	ImVec2 size;
	size.x = (float)santa_tex.width / 4.0f;
	size.y = (float)santa_tex.height / 4.0f;
	if (ImGui::Begin("Entity viewer", nullptr, 0))
	{
		//ImGui::InputInt("current_entity", &current_entity);
		size_t step = 1;
		size_t fast_step = 10;
		ImGui::InputScalar("", ImGuiDataType_U64, &current_entity, &step, &fast_step);

		auto& man = Entity_Manager::instance();
		auto& entities = man.get_entities();
		for (auto e : entities) {
			if (e.get_id() == current_entity) {
				ImGui::Text("Tag: %s", e.get_tag().c_str());
				if (e.has_component<C_Position>())
				{
					auto& pos = e.get_component<C_Position>();
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

void Bliss_App::render_dev_ui()
{
	Perf_Timer t(dev_ui.metrics.dev_ui_render_time);
	dev_ui.render();
}

void Bliss_App::render_scene()
{
	Perf_Timer t(dev_ui.metrics.drawing_time);
	EndDrawing();
}

void Bliss_App::load_fonts() {
	font_roboto_mono = LoadFontEx("data/fonts/RobotoMono-Regular.ttf", 28, nullptr, 255);
	font_roboto_mono_sm = LoadFontEx("data/fonts/RobotoMono-Regular.ttf", 12, nullptr, 255);
	font = LoadFontEx("data/fonts/Merriweather-Regular.ttf", 14, nullptr, 255);
}

void Bliss_App::load_textures() {
	santa_tex = LoadTexture("data/santa/Idle (1).png");
	santa_sm_tex = LoadTexture("data/santa/Idle (1) - Cropped - Small.png");
	popper_sm_tex = LoadTexture("data/santa/Idle (1) - Cropped - Small - Inverted.png");
	santa_cropped_tex = LoadTexture("data/santa/Idle (1) - Cropped.png");
	snowball_tex = LoadTexture("data/snowball/snowball_01_sm_mirrored.png");
}

Bliss_App::~Bliss_App()
{
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
