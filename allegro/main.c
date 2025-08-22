#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>

int main(){
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_FONT *font = NULL;
	if(!al_init()) {
		return -1;
	}
	display = al_create_display(640, 480);
	if(!display) {
		return -1;
	}
	al_init_font_addon();
	font = al_create_builtin_font();
	if(!font) {
		al_destroy_display(display);
		return -1;
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(255, 255, 255), 320, 240, ALLEGRO_ALIGN_CENTRE, "Hello, Allegro!");
	al_flip_display();
	al_rest(5.0);
	al_destroy_font(font);
	al_destroy_display(display);
	return 0;
}