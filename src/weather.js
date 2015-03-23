
// Location success can only take a single variable
// It was just simply to declare a global
var units;
var tomorrow = 0;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
    
  var url = "https://api.forecast.io/forecast//" + pos.coords.latitude + "," +
      pos.coords.longitude + "?units=" + units;
  
  var temperature;
  var icon;
  var forecast_icon;
  var high_temperature;
  var low_temperature;
  var city;
  
  // Send request to forecast.io
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Current temperature 
      temperature = Math.round(json.currently.temperature);
      console.log("Temperature is " + temperature);
      // weather icon
      icon = json.currently.icon;
      console.log("Weather icon: " + icon);
      // forecast icon
      forecast_icon = json.daily.data[tomorrow].icon;
      console.log("Forecast Icon: " + forecast_icon);
      // low and high temperatures
      low_temperature = Math.round(json.daily.data[tomorrow].temperatureMin);
      high_temperature = Math.round(json.daily.data[tomorrow].temperatureMax);    
      console.log("Hi: " + high_temperature + ", Low: " + low_temperature);
      
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_ICON": icon,
        "KEY_HIGH": high_temperature,
        "KEY_LOW": low_temperature,
        "KEY_FORECAST_ICON": forecast_icon,
        "KEY_TOMORROW": tomorrow
      };
      
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Weather info sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending weather info to Pebble!");
      }
      );
      
    }
  );
             
  url = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" +
    pos.coords.latitude + "," + pos.coords.longitude;
  
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      var address_components = json.results[0].address_components;
      for (var i=0; i < address_components.length; i++){
        if (address_components[i].types[0] == "locality") {
          city = address_components[i].long_name;
          break;
        }
      }
      // Send to Pebble
      var dictionary = {
        "KEY_CITY": city
      };

      // Send to Pebble, so we can load units variable and send it back
      Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Ready notice sent to phone!");
      },
        function(e) {
        console.log("Error ready notice to Pebble!");
      }
      );
    }
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    var dictionary = {
        "KEY_JSREADY": 1
    };

    // Send to Pebble, so we can load units variable and send it back
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Ready notice sent to phone!");
      },
      function(e) {
        console.log("Error ready notice to Pebble!");
      }
    ); 
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received");
    units = e.payload.KEY_UNITS;
    if (typeof units == 'undefined') units = "us";

    console.log("Units = " + units);
        var timeSwitch = e.payload.KEY_TIMESWITCH;
    
    //Is it currently after out timeswitch?
    if (typeof timeSwitch == 'undefined') timeSwitch = 12;
    var d = new Date();
    if (d.getHours() >= timeSwitch) {
      tomorrow = 1;
      console.log ("Tomorrow's Forecast!");
    } else {
      tomorrow = 0;
      console.log ("Today's Forecast!");
    }
    getWeather();
  }                     
);
