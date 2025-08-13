#include <imgui.h>

void draw_object_manager(ImGuiIO &io) {
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Object Manager");

	if (ImGui::Button("Button")) // Buttons return true when clicked (most
	                             // widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
	    1000.0f / io.Framerate,
	    io.Framerate);
	ImGui::End();
}
