#include <pebble.h>
  
#define KEY_JSREADY     0
#define KEY_UNITS       1
#define KEY_TEMPERATURE 2
#define KEY_CONDITIONS  3
  
#define MyTupletCString(_key, _cstring) \
((const Tuplet) { .type = TUPLE_CSTRING, .key = _key, .cstring = { .data = _cstring, .length = strlen(_cstring) + 1 }})


static Window *s_main_window; 

// variables
char w_units[] = "imperial";
int w_temp;
char w_conditions[32];

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
  //show_time();
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0){
    //update_weather();
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
      w_temp = t->value->int32;        
      APP_LOG(APP_LOG_LEVEL_INFO, "Temperature %d!", w_temp);
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

}

static void main_window_unload(Window *window) {
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
