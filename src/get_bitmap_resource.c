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
    return RESOURCE_ID_IMG_TORNADO;
    break;
  case 1:
    // Tropical Storms
    return RESOURCE_ID_IMG_RAIN;
    break;
  case 2:
    // Hurricane
    return RESOURCE_ID_IMG_HURRICANE;
    break;
  case 3:
    // Severe Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 4:
    // Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 5:
    // Mixed rain and snow
    return RESOURCE_ID_IMG_SNOW_RAIN;
    break;
  case 6:
    // Mixed rain and sleet
    return RESOURCE_ID_IMG_RAIN_FREEZING;
    break;
  case 7:
    // Mixed snow and sleet
    return RESOURCE_ID_IMG_SNOW_ICY;
    break;
  case 8:
    // Feezing drizzle
    return RESOURCE_ID_IMG_RAIN_FREEZING;
    break;
  case 9:
    // Drizzle
    return RESOURCE_ID_IMG_RAIN;
    break;
  case 10:
    // Freezing rain
    return RESOURCE_ID_IMG_RAIN_FREEZING;
    break;
  case 11:
    // Showers
    return RESOURCE_ID_IMG_RAIN;
    break;
  case 12:
    // Showers
    return RESOURCE_ID_IMG_RAIN;
    break;
  case 13:
    // Snow flurries
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 14:
    // Light snow showers
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 15:
    // Blowing snow
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 16:
    // Snow
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 17:
    // Hail
    return RESOURCE_ID_IMG_HAIL_HEAVY;
    break;
  case 18:
    // Sleet
    return RESOURCE_ID_IMG_RAIN_FREEZING;
    break;
  case 19:
    // Dust
    return RESOURCE_ID_IMG_DUST;
    break;
  case 20:
    // Foggy
    return RESOURCE_ID_IMG_FOG;
    break;
  case 21:
    // Haze
    return RESOURCE_ID_IMG_HAZE;
    break;
  case 22:
    // Smoky
    return RESOURCE_ID_IMG_SMOKE;
    break;
  case 23:
    // Blustery
    return RESOURCE_ID_IMG_WIND;
    break;
  case 24:
    // Windy
    return RESOURCE_ID_IMG_WIND;
    break;
  case 25:
    // Cold
    return RESOURCE_ID_IMG_TEMPERATURE_COLD;
    break;
  case 26:
    // Cloudy
    return RESOURCE_ID_IMG_CLOUDY;
    break;
  case 27:
    // Mostly cloudy night   
    return RESOURCE_ID_IMG_CLOUDY_MOSTLY_NIGHT;
    break;
  case 28:
    // Mostly cloudy day
    return RESOURCE_ID_IMG_CLOUDY_MOSTLY_DAY;
    break;
  case 29:
    // Partly cloudy night   
    return RESOURCE_ID_IMG_CLOUDY_NIGHT;
    break;
  case 30:
    // Partly cloudy day
    return RESOURCE_ID_IMG_CLOUDY_DAY;
    break;
  case 31:
    // Clear night
    return RESOURCE_ID_IMG_MOON;
    break;
  case 32:
    // Sunny
    return RESOURCE_ID_IMG_SUN;
    break;
  case 33:
    // fair night
    return RESOURCE_ID_IMG_MOON;
    break;
  case 34:
    // fair day
    return RESOURCE_ID_IMG_SUN;
    break;
  case 35:
    // mixed rain and hail
    return RESOURCE_ID_IMG_HAIL;
    break;
  case 36:
    // Hot
    return RESOURCE_ID_IMG_TEMPERATURE_HOT;
    break;
  case 37:
    // Isolated Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 38:
    // Scattered Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 39:
    // Scattered Thunderstorms
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 40:
    // Scattered Showers
    return RESOURCE_ID_IMG_RAIN;
  case 41:
    // Heavy snow
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 42:
    // Scattered snow showers
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 43:
    // Heavy snow
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 44:
    // PartLy cloudy
    return RESOURCE_ID_IMG_CLOUDY;
    break;
  case 45:
    // Thundershowers
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  case 46:
    // Snow showers
    return RESOURCE_ID_IMG_SNOW;
    break;
  case 47:
    // Isolated thundershowers
    return RESOURCE_ID_IMG_THUNDERSTORM;
    break;
  default:
    return RESOURCE_ID_IMG_UNKNOWN;
  }
    
}

/* I am sure there is abetter way to do this than an additional function
   and case, or 48 if statements in the existing function. But I do not
   know what it is */


// return the correct resource depending on icon name
int get_sm_bitmap(int code) {
  // Yahoo codes for weather icons.
  // We only have a subset of icons, so we will do ours best
  // https://developer.yahoo.com/weather/documentation.html#codes
  switch (code) {
  case 0:
    // Tornado (really should find icons for extreme weather)
    return RESOURCE_ID_SM_TORNADO;
    break;
  case 1:
    // Tropical Storms
    return RESOURCE_ID_SM_RAIN;
    break;
  case 2:
    // Hurricane
    return RESOURCE_ID_SM_HURRICANE;
    break;
  case 3:
    // Severe Thunderstorms
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 4:
    // Thunderstorms
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 5:
    // Mixed rain and snow
    return RESOURCE_ID_SM_SNOW_RAIN;
    break;
  case 6:
    // Mixed rain and sleet
    return RESOURCE_ID_SM_RAIN_FREEZING;
    break;
  case 7:
    // Mixed snow and sleet
    return RESOURCE_ID_SM_SNOW_ICY;
    break;
  case 8:
    // Feezing drizzle
    return RESOURCE_ID_SM_RAIN_FREEZING;
    break;
  case 9:
    // Drizzle
    return RESOURCE_ID_SM_RAIN;
    break;
  case 10:
    // Freezing rain
    return RESOURCE_ID_SM_RAIN_FREEZING;
    break;
  case 11:
    // Showers
    return RESOURCE_ID_SM_RAIN;
    break;
  case 12:
    // Showers
    return RESOURCE_ID_SM_RAIN;
    break;
  case 13:
    // Snow flurries
    return RESOURCE_ID_SM_SNOW;
    break;
  case 14:
    // Light snow showers
    return RESOURCE_ID_SM_SNOW;
    break;
  case 15:
    // Blowing snow
    return RESOURCE_ID_SM_SNOW;
    break;
  case 16:
    // Snow
    return RESOURCE_ID_SM_SNOW;
    break;
  case 17:
    // Hail
    return RESOURCE_ID_SM_HAIL_HEAVY;
    break;
  case 18:
    // Sleet
    return RESOURCE_ID_SM_RAIN_FREEZING;
    break;
  case 19:
    // Dust
    return RESOURCE_ID_SM_DUST;
    break;
  case 20:
    // Foggy
    return RESOURCE_ID_SM_FOG;
    break;
  case 21:
    // Haze
    return RESOURCE_ID_SM_HAZE;
    break;
  case 22:
    // Smoky
    return RESOURCE_ID_SM_SMOKE;
    break;
  case 23:
    // Blustery
    return RESOURCE_ID_SM_WIND;
    break;
  case 24:
    // Windy
    return RESOURCE_ID_SM_WIND;
    break;
  case 25:
    // Cold
    return RESOURCE_ID_SM_TEMPERATURE_COLD;
    break;
  case 26:
    // Cloudy
    return RESOURCE_ID_SM_CLOUDY;
    break;
  case 27:
    // Mostly cloudy night   
    return RESOURCE_ID_SM_CLOUDY_MOSTLY_NIGHT;
    break;
  case 28:
    // Mostly cloudy day
    return RESOURCE_ID_SM_CLOUDY_MOSTLY_DAY;
    break;
  case 29:
    // Partly cloudy night   
    return RESOURCE_ID_SM_CLOUDY_NIGHT;
    break;
  case 30:
    // Partly cloudy day
    return RESOURCE_ID_SM_CLOUDY_DAY;
    break;
  case 31:
    // Clear night
    return RESOURCE_ID_SM_MOON;
    break;
  case 32:
    // Sunny
    return RESOURCE_ID_SM_SUN;
    break;
  case 33:
    // fair night
    return RESOURCE_ID_SM_MOON;
    break;
  case 34:
    // fair day
    return RESOURCE_ID_SM_SUN;
    break;
  case 35:
    // mixed rain and hail
    return RESOURCE_ID_SM_HAIL;
    break;
  case 36:
    // Hot
    return RESOURCE_ID_SM_TEMPERATURE_HOT;
    break;
  case 37:
    // Isolated Thunderstorms
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 38:
    // Scattered Thunderstorms
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 39:
    // Scattered Thunderstorms
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 40:
    // Scattered Showers
    return RESOURCE_ID_SM_RAIN;
  case 41:
    // Heavy snow
    return RESOURCE_ID_SM_SNOW;
    break;
  case 42:
    // Scattered snow showers
    return RESOURCE_ID_SM_SNOW;
    break;
  case 43:
    // Heavy snow
    return RESOURCE_ID_SM_SNOW;
    break;
  case 44:
    // PartLy cloudy
    return RESOURCE_ID_SM_CLOUDY;
    break;
  case 45:
    // Thundershowers
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  case 46:
    // Snow showers
    return RESOURCE_ID_SM_SNOW;
    break;
  case 47:
    // Isolated thundershowers
    return RESOURCE_ID_SM_THUNDERSTORM;
    break;
  default:
    return RESOURCE_ID_SM_UNKNOWN;
  }
    
}
