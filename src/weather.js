
// Location success can only take a single variable
// It was just simply to declare a global
var units = "us";
var tomorrow = 0;


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
    console.log (city);
    woeid = json.query.results.Result.woeid;
    console.log (woeid);
    getWeather(woeid, city);
  };
  xhr.open('GET', url);
  xhr.send();

}

function getWeather(woeid, city) {  
  
  var temperature;
  var icon;
  var fIcon;
  var high;
  var low;
  var daylight;

  if (units == "us" || units == "f" ) {
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
    console.log ("Temperature is " + temperature);
    
    icon = parseInt(json.query.results.channel.item.condition.code);
    console.log ("Icon: " + icon);
    
    high = parseInt(json.query.results.channel.item.forecast[tomorrow].high);
    console.log ("High: " + high);
    
    low = parseInt(json.query.results.channel.item.forecast[tomorrow].low);
    console.log ("Low: " + low);
    
    fIcon = parseInt(json.query.results.channel.item.forecast[tomorrow].code);
    console.log ("Forecast icon: " + fIcon);
    
    // Is it day or night?
    var today = new Date();
    var yyyy = today.getFullYear();
    var mm = today.getMonth()+1;
    var dd = today.getDate();
    var sunrise = new Date(yyyy + '/' + mm + '/' + dd + ' ' + json.query.results.channel.astronomy.sunrise);
    var sunset = new Date(yyyy + '/' + mm + '/' + dd + ' ' + json.query.results.channel.astronomy.sunset);
    if (today > sunrise && today < sunset) {
      console.log ('Daytime');
      daylight = 1;
    } else {
      console.log ('Nighttime');
      daylight = 0;
    }
    console.log('Now: ' + today + ', Sunrise: ' + sunrise + ', Sunset:' + sunset);
    
    var dictionary = {
      'KEY_CITY': city,
      'KEY_ICON': icon,
      'KEY_HIGH': high,
      'KEY_LOW': low,
      'KEY_FORECAST_ICON': fIcon,
      'KEY_TEMPERATURE': temperature,
      'KEY_DAYLIGHT': daylight,
      'KEY_TOMORROW': tomorrow
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
    getLocation();
  }                     
);
