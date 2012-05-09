#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

void al_draw_vertical_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom);
void al_draw_horizontal_gradient_rect(float x, float y, float w, float h, ALLEGRO_COLOR left, ALLEGRO_COLOR right);
void al_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text);