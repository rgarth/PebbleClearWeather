Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  var units = localStorage.getItem('units');
  if (units in window) { units = "us" };
  var URL = 'http://rgarth.github.io/PebbleClearWeather/configuration.html?units=' + units;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL('http://rgarth.github.io/PebbleClearWeather/configuration.html?units=' + units);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var dictionary = {
        "KEY_UNITS": configuration.units
    };
    console.log('Configuration window returned: ' + configuration.units);
    localStorage.setItem('units', configuration.units);
    // Send to Pebble
    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Configuration sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending configuration info to Pebble!");
      }
    );
  }
); 