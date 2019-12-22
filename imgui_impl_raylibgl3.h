#include <assert.h>
#include <string.h>

//-----------opengl

//helper macro stolen from imgui.h
#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))
#define ImDrawCallback_ResetRenderState     (ImDrawCallback)(-1)

#define IM_ALLOC(_SIZE)                     igMemAlloc(_SIZE);
#define IM_FREE(_PTR)                       igMemFree(_PTR)

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*_ARR)))       // Size of a static C-style array. Don't use on pointers!
#define IM_UNUSED(_VAR)             ((void)_VAR)  

#define IM_ASSERT(_EXPR)            assert(_EXPR)

inline char* begin(ImVector_char* vec)
{ 
	return vec->Data; 
}

inline int _grow_capacity(ImVector_char* vec, int sz)
{
	int new_capacity = vec->Capacity ? (vec->Capacity + vec->Capacity / 2) : 8; 
	return new_capacity > sz ? new_capacity : sz; }

inline void reserve(ImVector_char* vec, int new_capacity)
{ 
	
	if (new_capacity <= vec->Capacity) return;
	void* new_data = (void*)IM_ALLOC((size_t)new_capacity * sizeof(char));
	if (vec->Data) {
		memcpy(new_data, vec->Data, (size_t)vec->Size * sizeof(char));
		IM_FREE(vec->Data);
	} 
	vec->Data = new_data; 
	vec->Capacity = new_capacity; 
}

inline void resize(ImVector_char* vec, int new_size)
{
	if (new_size > vec->Capacity) 
		reserve(vec, _grow_capacity(vec, new_size));
	
	vec->Size = new_size; 
}

// Backend API
bool     ImGui_ImplRaylibGL3_Init(const char* glsl_version);
void     ImGui_ImplRaylibGL3_Shutdown();
void     ImGui_ImplRaylibGL3_NewFrame();
void     ImGui_ImplRaylibGL3_RenderDrawData(ImDrawData* draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
bool     ImGui_ImplRaylibGL3_CreateFontsTexture();
void     ImGui_ImplRaylibGL3_DestroyFontsTexture();
bool     ImGui_ImplRaylibGL3_CreateDeviceObjects();
void     ImGui_ImplRaylibGL3_DestroyDeviceObjects();

#define IMGUI_IMPL_OPENGL_LOADER_GLAD