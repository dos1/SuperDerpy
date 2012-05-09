#include "allegro_utils.h"

void al_draw_vertical_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = top},
		{.x = x + w, .y = y, .z = 0, .color = top},
		{.x = x, .y = y + h, .z = 0, .color = bottom},
		{.x = x + w, .y = y + h, .z = 0, .color = bottom}};
		al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void al_draw_horizontal_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR left, ALLEGRO_COLOR right) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = left},
		{.x = x + w, .y = y, .z = 0, .color = right},
		{.x = x, .y = y + h, .z = 0, .color = left},
		{.x = x + w, .y = y + h, .z = 0, .color = right}};
		al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text) {
	al_draw_text(font, al_map_rgba(0,0,0,128), x+1, y+1, flags, text);
	al_draw_text(font, color, x, y, flags, text);
}
