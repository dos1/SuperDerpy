#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>

float FPS = 40;
int DISPLAY_WIDTH = 800;
int DISPLAY_HEIGHT = 500;

enum gamestate_enum {
	GAMESTATE_INTRO,
	GAMESTATE_MENU
};
enum gamestate_enum gamestate = GAMESTATE_INTRO;
enum gamestate_enum loadstate = GAMESTATE_MENU;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_BITMAP *loading_bitmap = NULL, *menu_bitmap = NULL, *cloud_bitmap = NULL, *pinkcloud_bitmap = NULL;
ALLEGRO_FONT *font;
float cloud_position = 100;

void PreloadGameState(int state) {	
	al_rest(1.0);
}

void LoadGameState(int state) {	
	if (state==GAMESTATE_MENU) {
		
		ALLEGRO_BITMAP *image = al_load_bitmap( "menu.png" );
		sample = al_load_sample( "menu.wav" );
		ALLEGRO_BITMAP *cloud = al_load_bitmap( "cloud.png" );
		ALLEGRO_BITMAP *pinkcloud = al_load_bitmap( "pinkcloud.png" );
		
		if (!sample){
			printf( "Audio clip sample not loaded!\n" ); 
			exit(-1);
		}
   
		// Scale menu bitmap
		menu_bitmap = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
		ALLEGRO_BITMAP *menu_fade_bitmap = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
		al_set_target_bitmap(menu_bitmap);
		al_draw_scaled_bitmap(image,0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), 0, 0, al_get_display_width(display), al_get_display_height(display),0);		 
		al_destroy_bitmap(image);

		al_set_target_bitmap(menu_fade_bitmap);
		al_clear_to_color(al_map_rgb(183,234,193));
		al_draw_bitmap(menu_bitmap,0, 0, 0);		 

		// Cloud menu bitmap
		cloud_bitmap = al_create_bitmap(al_get_display_width(display)*0.5, al_get_display_height(display)*0.25);
		al_set_target_bitmap(cloud_bitmap);
		al_draw_scaled_bitmap(cloud,0, 0, al_get_bitmap_width(cloud), al_get_bitmap_height(cloud), 0, 0, al_get_bitmap_width(cloud_bitmap), al_get_bitmap_height(cloud_bitmap),0);		 
		al_destroy_bitmap(cloud);
		
		pinkcloud_bitmap = al_create_bitmap(al_get_display_width(display)*0.33125, al_get_display_height(display)*0.8122);
		al_set_target_bitmap(pinkcloud_bitmap);
		al_draw_scaled_bitmap(pinkcloud,0, 0, al_get_bitmap_width(pinkcloud), al_get_bitmap_height(pinkcloud), 0, 0, al_get_bitmap_width(pinkcloud_bitmap), al_get_bitmap_height(pinkcloud_bitmap),0);		 
		al_destroy_bitmap(pinkcloud);

		al_set_target_bitmap(menu_fade_bitmap);
		al_draw_bitmap(pinkcloud_bitmap,al_get_display_width(display)*0.132, 0,0);
		al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(display)*0.5, al_get_display_height(display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");

		al_set_target_bitmap(al_get_backbuffer(display));
		
		al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		for(int fadeloop=0; fadeloop<256; fadeloop+=10){
			al_draw_tinted_bitmap(menu_fade_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		al_destroy_bitmap(menu_fade_bitmap);
	}
	gamestate = state;
}

int main(int argc, char **argv){
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP  *image = NULL;
   ALLEGRO_DISPLAY_MODE disp_data;
   bool redraw = true;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
   
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }

   if(!al_init_image_addon()) {
      fprintf(stderr, "failed to initialize image addon!\n");
      //al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
      //                           NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
   }

   if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
   }

   if(!al_install_keyboard()){
      fprintf(stderr, "failed to initialize keyboard!\n");
      return -1;
   }
   
   if(!al_init_acodec_addon()){
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
   }

   if (!al_reserve_samples(1)){
      fprintf(stderr, "failed to reserve samples!\n");
      return -1;
   }
   
   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon

   image = al_load_bitmap( "loading.png" );
   sample = al_load_sample( "menu.wav" );

   if (!sample){
      printf( "Audio clip sample not loaded!\n" ); 
      return -1;
   }
//   printf("%d\n", al_get_num_display_modes() );
   //al_get_display_mode(0, &disp_data);
   al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   display = al_create_display(800,500); //disp_data.width, disp_data.height); //DISPLAY_WIDTH, DISPLAY_HEIGHT);
   al_set_window_title(display, "Super Derpy: Muffins Attack");
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
   al_hide_mouse_cursor(display);
   font = al_load_ttf_font("ShadowsIntoLight.ttf",72,0 );
   
   // Scale "Loading" bitmap
   loading_bitmap = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
   al_set_target_bitmap(loading_bitmap);
   al_draw_scaled_bitmap(image,0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), 0, 0, al_get_display_width(display), al_get_display_height(display),0);		 
   al_draw_text(font, al_map_rgb(255,255,255), 30, al_get_display_height(display)-120, ALLEGRO_ALIGN_LEFT, "Loading...");
   al_set_target_bitmap(al_get_backbuffer(display));
   al_destroy_bitmap(image);

   
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      return -1;
   }

   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());

   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

   al_start_timer(timer);

   //al_rest(2.0);
   
   LoadGameState(GAMESTATE_INTRO);
   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);

      if(ev.type == ALLEGRO_EVENT_TIMER) {
         redraw = true;
      }
      else if((ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) || (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
         break;
      }
      
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
	 if (gamestate==GAMESTATE_INTRO) {
		for(int fadeloop=0; fadeloop<256; fadeloop+=10){
			al_draw_tinted_bitmap(loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		
		al_draw_bitmap(loading_bitmap,0,0,0);
		al_flip_display();
		
		PreloadGameState(loadstate);
		for(int fadeloop=255; fadeloop>0; fadeloop-=10){
			al_draw_tinted_bitmap(loading_bitmap,al_map_rgba_f(fadeloop/255.0,fadeloop/255.0,fadeloop/255.0,1),0,0,0);
			al_flip_display();
		}
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();
		al_rest(0.2);
		LoadGameState(loadstate);
	 }
	 else if (gamestate==GAMESTATE_MENU) {
		al_clear_to_color(al_map_rgb(183,234,193));
		al_draw_scaled_bitmap(cloud_bitmap,0,0,al_get_bitmap_width(cloud_bitmap), al_get_bitmap_height(cloud_bitmap), al_get_display_width(display)*(cloud_position-20)/(-75), al_get_display_height(display)*0.15, al_get_bitmap_width(cloud_bitmap)/2, al_get_bitmap_height(cloud_bitmap)/2,0);
		al_draw_bitmap(menu_bitmap,0, 0,0);
		al_draw_bitmap(pinkcloud_bitmap,(al_get_display_width(display)*0.132) + (cos((cloud_position/50+80)*1.74444))*20, 0,0);
		al_draw_bitmap(cloud_bitmap,al_get_display_width(display)*cloud_position/100, 30,0);
		al_draw_text(font, al_map_rgb(255,255,255), al_get_display_width(display)*0.5, al_get_display_height(display)*0.2, ALLEGRO_ALIGN_CENTRE, "Super Derpy");
		cloud_position-=0.2;
		if (cloud_position<-80) cloud_position=100;
		al_flip_display();
	}
      }
   }

   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_bitmap(loading_bitmap);
   al_destroy_event_queue(event_queue);
   al_destroy_sample(sample);

   return 0;
}
