#include <pebble.h>

static Window *window;
static GBitmap *GF_BM;
static BitmapLayer *GF_BML;
static TextLayer *text_time_layer;

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
	static char time_text[14];
	char *time_format;
	
	//format and set hour text
  if (clock_is_24h_style()) {
    time_format = "%Hh\n\n%Mm\n\n%Ss";
  } else {
    time_format = "%Ih\n\n%Mm\n\n%Ss";
  }
	
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  text_layer_set_text(text_time_layer, time_text);
}

static void load_gf_image()
{
	window = window_create();
    window_stack_push(window, true);
	
	GF_BM = gbitmap_create_with_resource(RESOURCE_ID_gf);
	GRect frame = (GRect) {
		.origin = { 0,0 },
		.size = GF_BM->bounds.size
	};
	//creat image layer
	BitmapLayer *bitmap_layer = bitmap_layer_create(frame);
	GF_BML = bitmap_layer;
	bitmap_layer_set_bitmap(bitmap_layer, GF_BM);
	
	//create time layer
	text_time_layer = text_layer_create(GRect(109, 3, 34, 140)); 
	text_layer_set_text_color(text_time_layer, GColorBlack);
    text_layer_set_background_color(text_time_layer, GColorWhite);
    text_layer_set_font(text_time_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_24));

	//add layers to root layer
	Layer *window_layer = window_get_root_layer(window);
	layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));
	layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
	
	//setup callback for time
	tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
}

static void unload_gf_image()
{
	layer_remove_from_parent(bitmap_layer_get_layer(GF_BML));
    bitmap_layer_destroy(GF_BML);
    gbitmap_destroy(GF_BM);
	window_destroy(window);
	
	tick_timer_service_unsubscribe();
}

int main(void) {
  load_gf_image();
  app_event_loop();
  unload_gf_image();
}
