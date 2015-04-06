#include <pebble.h>
#include "get_bitmap_resource.h"

#define KEY_JSREADY       0
#define KEY_UNITS         1
#define KEY_TIMESWITCH    2
#define KEY_TEMPERATURE   3
#define KEY_ICON          4
#define KEY_HIGH          5
#define KEY_LOW           6
#define KEY_FORECAST_ICON 7
#define KEY_CITY          8
#define KEY_TOMORROW      9
#define KEY_LAST         10
#define KEY_DAYLIGHT     11
  
#define MyTupletCString(_key, _cstring) \
((const Tuplet) { .type = TUPLE_CSTRING, .key = _key, .cstring = { .data = _cstring, .length = strlen(_cstring) + 1 }})

// Main Window
static Window *s_main_window;
static TextLayer *s_time_layer, *s_temperature_layer;
static BitmapLayer *s_bitmap_layer;
static Layer *w_bg_layer;

// Forecast Window
static Window *s_forecast_window;
static TextLayer *s_forecast_header_layer, *s_forecast_high_layer, *s_forecast_low_layer;
static BitmapLayer *s_forecast_bitmap_layer;
static InverterLayer *s_forecast_invert_layer;
static Layer *s_forecast_bg_layer;
// variables
char w_units[] = "us";
int w_current, f_high, f_low;
int time_switch = 12;
char f_city[32], w_header[64];
int w_icon = -1, f_icon = -1;
GBitmap *w_bitmap, *f_bitmap;
GFont weather_font, time_font, header_font, hilo_font;
bool forecast_face = 0;
bool tomorrow = 0;
AppTimer *shake_timer;
int last_update;
bool daylight = 1;


static void show_temperature() {
  if (w_current) {
    static char buffer[6];
    snprintf(buffer, sizeof(buffer), "%i\u00B0", w_current);
    text_layer_set_text(s_temperature_layer, buffer);
  }
}

static void w_update_bg_color(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Updating background");
  GRect bounds = layer_get_bounds(layer);

  // Draw a black filled rectangle with sharp corners
  #ifdef PBL_COLOR
  if (daylight) {
    graphics_context_set_fill_color(ctx, GColorPictonBlue);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  } else {
    graphics_context_set_fill_color(ctx, GColorLiberty);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);    
  }
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  #endif
}
 
static void show_icon() {
  if (w_icon >= 0) {
    gbitmap_destroy(w_bitmap);
    w_bitmap = gbitmap_create_with_resource(get_bitmap(w_icon));
    bitmap_layer_set_bitmap(s_bitmap_layer, w_bitmap);
  }
  layer_mark_dirty(w_bg_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Marking background dirty");
}

static void show_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    // Want to strip the leading 0. Not just replace it with a *space*
    // which is what %l does
    if (buffer[0] == '0') 
      memmove(buffer, buffer+1, strlen(buffer));
  }
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);

}

static void update_weather() {
  // Only grab the weather if we can talk to phone
  if (bluetooth_connection_service_peek()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is connected!");
    
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet dictionary[] = {
      MyTupletCString(KEY_UNITS, w_units),
      TupletInteger(KEY_TIMESWITCH, time_switch),
    };
    dict_write_tuplet(iter, &dictionary[0]);
    dict_write_tuplet(iter, &dictionary[1]);
    app_message_outbox_send();
  } 
}

static void bt_handler(bool connected) {
  if (connected) {
    vibes_short_pulse();
    update_weather();
    
  } else {
    vibes_short_pulse(); 
  }
  
}

static void shake_timer_callback(void *date) {
  if (window_stack_contains_window(s_forecast_window)) {
    window_stack_pop(1);
    forecast_face = 0;
  } else {
    window_stack_push(s_forecast_window, 1);
    app_timer_register(4000, (AppTimerCallback) shake_timer_callback, NULL);
  }
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  // If weather is stale update weather
  if ((time(NULL) - 3600) > last_update) update_weather();

  // Don't start a second timer if we are already running one
  if (! forecast_face) {
    if (f_high) { 
      forecast_face = 1;
      shake_timer = app_timer_register(2000, (AppTimerCallback) shake_timer_callback, NULL);    
    } else {
      // temp may be from persistent storage
      update_weather();
    }   
  }

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  show_time();

  if (! w_current) {
    update_weather();
  }
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0){
    update_weather();
  }

}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // Do we need to update the weather again?
  bool need_weather = 0;
  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_UNITS:
      // units returned from configuration.js
      // we should regrab the weather in the correct units
      snprintf(w_units, sizeof(w_units), "%s", t->value->cstring);
      persist_write_string(KEY_UNITS, w_units);
      need_weather = 1;
      break;
    case KEY_TIMESWITCH:
      // time changes, we may need to get hi and low again
      time_switch = t->value->int32;
      persist_write_int(KEY_TIMESWITCH, time_switch);
      need_weather = 1;
      break;
    case KEY_TEMPERATURE:
      w_current = t->value->int32;
      last_update = time(NULL);
      persist_write_int(KEY_TEMPERATURE, w_current);
      persist_write_int(KEY_LAST, last_update);
      show_temperature();
      break;
    case KEY_ICON:
      w_icon = t->value->int32;
      persist_write_int(KEY_ICON, w_icon);
      show_icon();
      break;
    case KEY_HIGH:
      f_high = t->value->int32;
      break;
    case KEY_LOW:
      f_low = t->value->int32;
      break;
    case KEY_FORECAST_ICON:
      f_icon = t->value->int32;
      break;
    case KEY_TOMORROW:
      tomorrow = t->value->int16;
      break;
    case KEY_DAYLIGHT:
      daylight = t->value->int16;
      show_icon();
      break;
    case KEY_CITY:
      snprintf(f_city, sizeof(f_city), "%s", t->value->cstring);
      break;
    case KEY_JSREADY:
      // JS ready lets get the weather
      if (t->value->int16) {
        need_weather = 1;
      }
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }   
    
    // Look for next item
    t = dict_read_next(iterator);
  }
  if (need_weather) {
    update_weather();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
} 


static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Loaded main windows");
 
  // Colored bitmap for weather bg
  w_bg_layer = layer_create(GRect(0, 84, 144, 84));
  layer_set_update_proc(w_bg_layer, w_update_bg_color); 
  layer_add_child(window_get_root_layer(window), w_bg_layer);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 12, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIDACTIC_GOTHIC_48)); 
  text_layer_set_font(s_time_layer, time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
       
  // temperature layer
  s_temperature_layer = text_layer_create(GRect(72, 104, 72, 40));
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  text_layer_set_text_color(s_temperature_layer, GColorWhite); 
  text_layer_set_overflow_mode(s_temperature_layer, GTextOverflowModeWordWrap);
  weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIDACTIC_GOTHIC_36)); 
  text_layer_set_font(s_temperature_layer, weather_font);
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_temperature_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Until we have the temperature
  text_layer_set_text(s_temperature_layer, "--\u00B0");

  // Icon layer
  s_bitmap_layer = bitmap_layer_create(GRect(0, 84, 72, 84));
  bitmap_layer_set_alignment(s_bitmap_layer, GAlignCenter);
  bitmap_layer_set_background_color(s_bitmap_layer, GColorClear);
  //bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);

  #ifdef PBL_COLOR
  w_bitmap = gbitmap_create_blank(GSize(72, 72), GBitmapFormat8Bit);
  #else
  w_bitmap = gbitmap_create_blank(GSize(72, 72));
  #endif

  bitmap_layer_set_bitmap(s_bitmap_layer, w_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
    

} 

static void main_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Unloaded main window");
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(weather_font);
  text_layer_destroy(s_time_layer);
    
  text_layer_destroy(s_temperature_layer);
  gbitmap_destroy(w_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
  layer_destroy(w_bg_layer);
}

static void forecast_window_load(Window *window) {
  // Every time the window gets pushed it will load
  APP_LOG(APP_LOG_LEVEL_INFO, "Loaded forecast window");
  
  // icon
  s_forecast_bitmap_layer = bitmap_layer_create(GRect(0, 0, 44, 84));
  bitmap_layer_set_alignment(s_forecast_bitmap_layer, GAlignCenter);
  bitmap_layer_set_background_color(s_forecast_bitmap_layer, GColorBlack);
  f_bitmap = gbitmap_create_with_resource(get_sm_bitmap(f_icon));
  bitmap_layer_set_bitmap(s_forecast_bitmap_layer, f_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_forecast_bitmap_layer));
  
  // header
  s_forecast_header_layer = text_layer_create(GRect(44, 0, 100, 84));
  text_layer_set_background_color(s_forecast_header_layer, GColorBlack);
  text_layer_set_text_color(s_forecast_header_layer, GColorWhite);
  text_layer_set_overflow_mode(s_forecast_header_layer, GTextOverflowModeWordWrap);
  header_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  text_layer_set_font(s_forecast_header_layer, header_font);
  text_layer_set_text_alignment(s_forecast_header_layer, GTextAlignmentLeft);
  static char header[64];
  if (tomorrow) {
    snprintf(header, sizeof(header), "Tomorrow %s", f_city);
  } else {
    snprintf(header, sizeof(header), "Today %s", f_city);
  }
  text_layer_set_text(s_forecast_header_layer, header);
  // header valign center
  GSize text_size = (text_layer_get_content_size(s_forecast_header_layer));
  int text_height = text_size.h + 5;
  int y_origin = (82/2) - (text_height/2) - 1;
  if (y_origin >= 0) {
    text_layer_set_size(s_forecast_header_layer, text_size);
    layer_set_frame(text_layer_get_layer(s_forecast_header_layer), GRect(48, y_origin, 100, text_height));
  }
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_forecast_header_layer));  

  // flip the bottom half of the window to white
  s_forecast_invert_layer = inverter_layer_create(GRect(0, 84, 144, 84));
  layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(s_forecast_invert_layer));

  // grey background
  s_forecast_bg_layer = layer_create(GRect(72, 84, 144, 84));
  layer_set_update_proc(s_forecast_bg_layer, w_update_bg_color); 
  layer_add_child(window_get_root_layer(window), s_forecast_bg_layer);

  // high and low
  hilo_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIDACTIC_GOTHIC_36));
  s_forecast_high_layer = text_layer_create(GRect(0, 102, 72, 40));
  text_layer_set_background_color(s_forecast_high_layer, GColorClear);
  text_layer_set_text_color(s_forecast_high_layer, GColorBlack);
  text_layer_set_overflow_mode(s_forecast_high_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(s_forecast_high_layer, hilo_font);
  text_layer_set_text_alignment(s_forecast_high_layer, GTextAlignmentCenter);
  static char h[5];
  snprintf(h, sizeof(h), "%i", f_high);
  text_layer_set_text(s_forecast_high_layer, h);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_forecast_high_layer));


  s_forecast_low_layer = text_layer_create(GRect(72, 102, 72, 40));
  text_layer_set_background_color(s_forecast_low_layer, GColorClear);
  text_layer_set_text_color(s_forecast_low_layer, GColorWhite);
  text_layer_set_overflow_mode(s_forecast_low_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(s_forecast_low_layer, hilo_font);
  text_layer_set_text_alignment(s_forecast_low_layer, GTextAlignmentCenter);
  static char l[5];
  snprintf(l, sizeof(l), "%i", f_low);
  text_layer_set_text(s_forecast_low_layer, l); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_forecast_low_layer));
 
}

static void forecast_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Unoaded forecast window");
  gbitmap_destroy(f_bitmap);
  bitmap_layer_destroy(s_forecast_bitmap_layer);
  text_layer_destroy(s_forecast_header_layer);
  inverter_layer_destroy(s_forecast_invert_layer);
  layer_destroy(s_forecast_bg_layer);
  fonts_unload_custom_font(hilo_font);
  text_layer_destroy(s_forecast_high_layer);
  text_layer_destroy(s_forecast_low_layer);
}

static void init () {
  // Load stored values
  if (persist_exists(KEY_UNITS)) {
    persist_read_string(KEY_UNITS, w_units, sizeof(w_units));
    APP_LOG(APP_LOG_LEVEL_INFO, "Reading temperature units: %s", w_units);
  }
  if (persist_exists(KEY_TIMESWITCH)) {
    time_switch = persist_read_int(KEY_TIMESWITCH);
    APP_LOG(APP_LOG_LEVEL_INFO, "Reading time switch value: %i", time_switch);
  }
  if (persist_exists(KEY_LAST)) {
    last_update = persist_read_int(KEY_LAST);
    // Use the last found weather is less the 90 minutes old
    if ((time(NULL) - 5400) < last_update) {
      if (persist_exists(KEY_TEMPERATURE)) {
        w_current = persist_read_int(KEY_TEMPERATURE);
        APP_LOG(APP_LOG_LEVEL_INFO, "Reading last good temperature: %i", w_current);
      }
      if (persist_exists(KEY_ICON)) {
        w_icon = persist_read_int(KEY_ICON);
        APP_LOG(APP_LOG_LEVEL_INFO, "Reading last good icon: %i", w_icon);
      }
    }   
  }

  // Create main Window element and assign to pointer
  s_main_window = window_create();  
  window_set_background_color(s_main_window, GColorBlack); 
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Create forecast Window elemnent, but do not push it
  s_forecast_window = window_create();
  window_set_background_color(s_forecast_window, GColorBlack);
  window_set_window_handlers(s_forecast_window, (WindowHandlers) {
    .load = forecast_window_load,
    .unload = forecast_window_unload    
  });
   
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register with  AccelTimeService
  // A time will show to date
  accel_tap_service_subscribe(tap_handler); 
  
  // Register with the BluetoothConnectionService
  bluetooth_connection_service_subscribe(bt_handler); 
  
  // Make sure the time is displayed from the start
  show_time();
  show_temperature();
  show_icon();
  
}

static void deinit () {
  // Destroy Window
  window_destroy(s_main_window);
  window_destroy(s_forecast_window);
  app_message_deregister_callbacks();
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe(); 
  bluetooth_connection_service_unsubscribe(); 

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
