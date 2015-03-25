#include <pebble.h>
#include "get_bitmap_resource.h"

// return the correct resource depending on icon name
int get_bitmap(int code) {
  // Yahoo codes for weather icons.
  // We only have a subset of icons, so we will do ours best
  // https://developer.yahoo.com/weather/documentation.html#codes
  switch (code) {
  case 0:
    // Tornado (really should find icons for extreme weather)
    return RESOURCE_ID_IMG_WIND_WHITE;
    break;
  case 1:
    // Tropical Storms
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 2:
    // Hurricane
    return RESOURCE_ID_IMG_WIND_WHITE;
    break;
  case 3:
    // Severe Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 4:
    // Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 5:
    // Mixed rain and snow
    return RESOURCE_ID_IMG_SLEET_WHITE;
    break;
  case 6:
    // Mixed rain and sleet
    return RESOURCE_ID_IMG_SLEET_WHITE;
    break;
  case 7:
    // Mixed snow and sleet
    return RESOURCE_ID_IMG_SLEET_WHITE;
    break;
  case 8:
    // Feezing drizzle
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 9:
    // Drizzle
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 10:
    // Freezing rain
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 11:
    // Showers
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 12:
    // Showers
    return RESOURCE_ID_IMG_RAIN_WHITE;
    break;
  case 13:
    // Snow flurries
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 14:
    // Light snow showers
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 15:
    // Blowing snow
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 16:
    // Snow
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 17:
    // Hail
    return RESOURCE_ID_IMG_HAIL_WHITE;
    break;
  case 18:
    // Sleet
    return RESOURCE_ID_IMG_SLEET_WHITE;
    break;
  case 19:
    // Dust
    return RESOURCE_ID_IMG_HAZE_WHITE;
    break;
  case 20:
    // Foggy
    return RESOURCE_ID_IMG_FOG_WHITE;
    break;
  case 21:
    // Haze
    return RESOURCE_ID_IMG_HAZE_WHITE;
    break;
  case 22:
    // Smoky
    return RESOURCE_ID_IMG_HAZE_WHITE;
    break;
  case 23:
    // Blustery
    return RESOURCE_ID_IMG_WIND_WHITE;
    break;
  case 24:
    // Windy
    return RESOURCE_ID_IMG_WIND_WHITE;
    break;
  case 25:
    // Cold
    return RESOURCE_ID_IMG_COLD_WHITE;
    break;
  case 26:
    // Cloudy
    return RESOURCE_ID_IMG_CLOUDY_WHITE;
    break;
  case 27:
    // Mostly cloudy night   
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_NIGHT_WHITE;
    break;
  case 28:
    // Mostly cloudy day
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_DAY_WHITE;
    break;
  case 29:
    // Partly cloudy night   
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_NIGHT_WHITE;
    break;
  case 30:
    // Partly cloudy day
    return RESOURCE_ID_IMG_PARTLY_CLOUDY_DAY_WHITE;
    break;
  case 31:
    // Clear night
    return RESOURCE_ID_IMG_CLEAR_NIGHT_WHITE;
    break;
  case 32:
    // Sunny
    return RESOURCE_ID_IMG_CLEAR_DAY_WHITE;
    break;
  case 33:
    // fair night
    return RESOURCE_ID_IMG_CLEAR_NIGHT_WHITE;
    break;
  case 34:
    // fair day
    return RESOURCE_ID_IMG_CLEAR_DAY_WHITE;
    break;
  case 35:
    return RESOURCE_ID_IMG_HAIL_WHITE;
    break;
  case 36:
    // Hot
    return RESOURCE_ID_IMG_HOT_WHITE;
    break;
  case 37:
    // Isolated Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 38:
    // Scattered Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 39:
    // Scattered Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 40:
    // Scattered Showers
    return RESOURCE_ID_IMG_RAIN_WHITE;
  case 41:
    // Heavy snow
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 42:
    // Scattered snow showers
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 43:
    // Heavy snow
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 44:
    // Party cloudy
    return RESOURCE_ID_IMG_CLOUDY_WHITE;
    break;
  case 45:
    // Thundershowers
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  case 46:
    // Snow showers
    return RESOURCE_ID_IMG_SNOW_WHITE;
    break;
  case 47:
    // Isolated thundershowers
    return RESOURCE_ID_IMG_THUNDERSTORM_WHITE;
    break;
  default:
    return RESOURCE_ID_IMG_UNKNOWN_WHITE;
  }
    
}
