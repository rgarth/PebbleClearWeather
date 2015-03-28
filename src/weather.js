
// Location success can only take a single variable
// It was just simply to declare a global
var units = "us";
var tomorrow = 0;
var dictionary = new Object;


function locationSuccess(pos) {
  // We neeed to get the Yahoo woeid first
  var woeid;
  var city;

  var query = 'select * from geo.placefinder where text="' +
    pos.coords.latitude + ',' + pos.coords.longitude + '" and gflags="R"';
  console.log(query);
  var url = 'https://query.yahooapis.com/v1/public/yql?q=' + query + '&format=json';
  console.log(url);
  // Send request to Yahoo
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    var json = JSON.parse(this.responseText);
    city = json.query.results.Result.city;
    dictionary["KEY_CITY"] = city;
    console.log (city);
    woeid = json.query.results.Result.woeid;
    console.log (woeid);
    getWeather(woeid);
  };
  xhr.open('GET', url);
  xhr.send();

}

function getWeather(woeid) {  
  
  var temperature;
  var icon;
  var fIcon;
  var high;
  var low;

  if (units == "us" || units == "f") {
    units = "f";
  } else {
    units = "c";
  }
  
  var query = 'select * from weather.forecast where woeid = ' + woeid + ' and u="' + units + '"';
  console.log(query);
  var url = 'https://query.yahooapis.com/v1/public/yql?q=' + query + '&format=json&env=store://datatables.org/alltableswithkeys';
  console.log(url);
  // Send request to Yahoo
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    var json = JSON.parse(this.responseText);
    temperature = parseInt(json.query.results.channel.item.condition.temp);
    dictionary["KEY_TEMPERATURE"] = temperature;
    console.log ("Temperature is " + temperature);
    
    icon = parseInt(json.query.results.channel.item.condition.code);
    dictionary["KEY_ICON"] = icon;
    console.log ("Icon: " + icon);
    
    high = parseInt(json.query.results.channel.item.forecast[tomorrow].high);
    dictionary["KEY_HIGH"] = high;
    console.log ("High: " + high);
    
    low = parseInt(json.query.results.channel.item.forecast[tomorrow].low);
    dictionary["KEY_LOW"] = low;
    console.log ("Low: " + low);
    
    fIcon = parseInt(json.query.results.channel.item.forecast[tomorrow].code);
    dictionary["KEY_FORECAST_ICON"] = fIcon;
    console.log ("Forecast icon: " + fIcon);
    
    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log("Weather info sent to Pebble successfully!");
    },
    function(e) {
      console.log("Error sending weather info to Pebble!");
    }
    );
  };
  xhr.open('GET', url);
  xhr.send();
}

function locationError(err) {
  console.log("Error requesting location!");
}

// Get Location lat+lon
function getLocation() {
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
    dictionary["KEY_TOMORROW"] = tomorrow;
    getLocation();
  }                     
);
