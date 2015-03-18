
// Location success can Inly take a single variable
// It was just simply to declare a global
var units = "us";

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
    
  var url = "https://api.forecast.io/forecast/8af757cd68c3cacf3de9ee497051c0a5/" + pos.coords.latitude + "," +
      pos.coords.longitude + "?units=" + units;
  console.log("URL is " + url);

  
  var conditions;
  var temperature;
  var icon;
  var low_temperature;
  var high_temperature;
  var city;
  
  // Send request to forecast.io
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Current temperature 
      temperature = Math.round(json.currently.temperature);
      console.log("Temperature is " + temperature);

      // Conditions
      conditions = json.currently.summary;      
      console.log("Conditions are " + conditions);
      
      // weather icon
      icon = json.currently.icon;
      console.log("Weather icon, " + icon);
      
      // low and high temperatures
      low_temperature = Math.round(json.daily.data[0].temperatureMin);
      high_temperature = Math.round(json.daily.data[0].temperatureMax);
      console.log("High:" + high_temperature + ", Low:" + low_temperature);
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions,
        "KEY_HIGH": high_temperature,
        "KEY_LOW": low_temperature,
        "KEY_ICON": icon
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
  console.log("URL is " + url);
  
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      var address_components = json.results[0].address_components;

      for (var i=0; i < address_components.length; i++){
        if (address_components[i].types[0] == "locality") {
          city = address_components[i].long_name;
          console.log("City is " + city);        
          // Send to Pebble
          var dictionary = {
            "KEY_CITY": city
          };
          Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("Location data sent to Pebble successfully!");
          },
          function(e) {
            console.log("Error sending location data to Pebble!");
          }
          );
        }     
      }

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
 

    // Get the initial weather
    //getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received");
    units = e.payload.KEY_UNITS;
    if (typeof units == 'undefined') {
      units = "us";
    }
    getWeather();
  }                     
);