#include <pebble.h>
	
static Window *window;
static TextLayer *fact_text;
static TextLayer *date_text;
static ScrollLayer *scroll_layer;
static InverterLayer *inverter_layer;
static const int vert_scroll_text_padding = 4;

static void in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *fact_tuple = dict_find(iter, 1);
	Tuple *date_tuple = dict_find(iter, 2);
	text_layer_set_text(fact_text, fact_tuple->value->cstring);
	GSize max_size = text_layer_get_content_size(fact_text);
	text_layer_set_size(fact_text, max_size);
	scroll_layer_set_content_size(scroll_layer, GSize(148, max_size.h + vert_scroll_text_padding));
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(fact_text));
	text_layer_set_text(date_text, date_tuple->value->cstring);
}

static void app_message_init(void) {
	app_message_register_inbox_received(in_received_handler);
	app_message_open(256, 256);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect max_text_bounds = GRect(5, 0, 140, 2000);
	scroll_layer = scroll_layer_create(GRect(0, 40, 148, 112));
	scroll_layer_set_click_config_onto_window(scroll_layer, window);
	fact_text = text_layer_create(max_text_bounds);
	text_layer_set_text_alignment(fact_text, GTextAlignmentLeft);
	text_layer_set_font(fact_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
	date_text = text_layer_create(GRect(0, 0, 148, 35));
	text_layer_set_text_alignment(date_text, GTextAlignmentCenter);
	text_layer_set_font(date_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(window_layer, text_layer_get_layer(date_text));
	text_layer_set_text(date_text, "Loading...");
	inverter_layer = inverter_layer_create(GRect(0, 0, 148, 40));
	layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
}

static void window_unload(Window *window) {
	text_layer_destroy(fact_text);
	scroll_layer_destroy(scroll_layer);
	text_layer_destroy(date_text);
	inverter_layer_destroy(inverter_layer);
}

static void init(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});
	const bool animated = true;
	window_stack_push(window, animated);
}

static void deinit(void) {
	window_destroy(window);
}

int main(void) {
	init();
	app_message_init();
	app_event_loop();
	deinit();
}