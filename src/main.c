#include <pebble.h>
  
#define KEY_JSREADY     0
#define KEY_UNITS       1
#define KEY_TEMPERATURE 2
#define KEY_CONDITIONS  3
#define KEY_HIGH        4
#define KEY_LOW         5
#define KEY_ICON        6
#define KEY_CITY        7
  
#define MyTupletCString(_key, _cstring) \
((const Tuplet) { .type = TUPLE_CSTRING, .key = _key, .cstring = { .data = _cstring, .length = strlen(_cstring) + 1 }})


static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_temperature_layer;
static BitmapLayer *s_bitmap_layer;

// variables
char w_units[] = "us";
int w_current;
int w_high;
int w_low;
char w_conditions[32];
char w_icon[32];
char w_city[32];
GBitmap *w_bitmap;

static int get_bitmap(char str[]) {
  if (strcmp(str, "clear-day") == 0)
    return RESOURCE_ID_IMG_CLEAR_DAY_WHITE;    
  else if (strcmp(str, "clear-night") == 0) 
    return RESOURCE_ID_IMG_CLEAR_NIGHT_WHITE;
  else if (strcmp(str, "cloudy") == 0)
    return RESOURCE_ID_IMG_CLOUDY_WHITE;
  else if (strcmp(str, "fog") == 0)
    return RESOURCE_ID_IMG_FOG_WHITE;
  else if (strcmp(str, "hail") == 0)
    return RESOURCE_ID_IMG_HAIL_WHITE;
  else if (strcmp(str, "partly-cloudy-day") == 0)
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_DAY_WHITE;
  else if (strcmp(str, "partly-cloudy-night") == 0)
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_NIGHT_WHITE;
  else if (strcmp(str, "rain") == 0)
    return RESOURCE_ID_IMG_RAIN_WHITE);
  else if (strcmp(str, "sleet") == 0)
    return RESOURCE_ID_IMG_SLEET_WHITE);
  else if (strcmp(str, "snow") == 0)
    return RESOURCE_ID_IMG_SNOW_WHITE);
  else if (strcmp(str, "thunderstorm") == 0)
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE);
  else if (strcmp(str, "tornado") == 0)
    return RESOURCE_ID_IMG_TORNADO_WHITE);
  else if (strcmp(str, "wind") == 0)
    return RESOURCE_ID_IMG_WIND_WHITE);
}

static void show_temperature() {
  if (w_current) {
    static char buffer[5];
    snprintf(buffer, 5, "%i\u00B0", w_current);
    text_layer_set_text(s_temperature_layer, buffer);
  }
}

static void show_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
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
    
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    Tuplet tuple = MyTupletCString(KEY_UNITS, w_units);

    // Add a key-value pair
    dict_write_tuplet(iter, &tuple);

    // Send the message!
    app_message_outbox_send();
    
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is not connected!");
  } 
}

static void bt_handler(bool connected) {
  if (connected) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone has connected!");
    vibes_short_pulse();
    //update_weather();
    
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone has disconnected!");
    vibes_short_pulse(); 
  }
  
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  show_time();
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0){
    update_weather();
  }

}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      w_current = t->value->int32;        
      APP_LOG(APP_LOG_LEVEL_INFO, "Temperature %d!", w_current);
      show_temperature();
      break;
    case KEY_CONDITIONS:
      snprintf(w_conditions, sizeof(w_conditions), "%s", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Conditions %s!", w_conditions);
      break;
    case KEY_UNITS:
      // units returned from configuration.js
      // we should regrab the weather in the correct units
      snprintf(w_units, sizeof(w_units), "%s", t->value->cstring);
      persist_write_string(KEY_UNITS, w_units);
      APP_LOG(APP_LOG_LEVEL_INFO, "Storing temperature units: %s", w_units);
      update_weather();
      break;
    case KEY_HIGH:
      w_high = t->value->int32;
      APP_LOG(APP_LOG_LEVEL_INFO, "High %d!", w_high);
      break;
    case KEY_LOW:
      w_low = t->value->int32;
      APP_LOG(APP_LOG_LEVEL_INFO, "Low %d!", w_low);
      break;
    case KEY_ICON:
      snprintf(w_icon, sizeof(w_icon), "%s", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Icon %s!", w_icon);
      w_bitmap = gbitmap_create_with_resource(get_bitmap(w_icon));
      if (w_bitmap)
        bitmap_layer_set_bitmap(s_bitmap_layer, w_bitmap);
      break;
    case KEY_CITY:
      snprintf(w_city, sizeof(w_city), "%s", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "Location: %s", t->value->cstring);
      break;
    case KEY_JSREADY:
      APP_LOG(APP_LOG_LEVEL_INFO, "PebbleJS is ready!");
      if (t->value->int16) {
        update_weather();
      }
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }   

    // Look for next item
    t = dict_read_next(iterator);
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Finished received message");
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
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 22, 144, 30));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
  s_temperature_layer = text_layer_create(GRect(72, 107, 72, 30));
  text_layer_set_background_color(s_temperature_layer, GColorBlack);
  text_layer_set_text_color(s_temperature_layer, GColorWhite);
  
  GFont custom_font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK); 

  text_layer_set_font(s_time_layer, custom_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  text_layer_set_font(s_temperature_layer, custom_font);
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_temperature_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  // Until we have the temperature
  text_layer_set_text(s_temperature_layer, "--\u00B0");

  s_bitmap_layer = bitmap_layer_create(GRect(0, 84, 72, 84));
  bitmap_layer_set_background_color(s_bitmap_layer, GColorBlack); 
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_temperature_layer);
  bitmap_layer_destroy(s_bitmap_layer);
} 

static void init () {
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();  
  window_set_background_color(s_main_window, GColorBlack); 

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

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

}

static void deinit () {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
