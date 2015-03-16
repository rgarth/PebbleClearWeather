Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  console.log('Configuration window opened.');
  Pebble.openURL('http://rgarth.github.io/PebbleClearWeather/configuration.html');
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var dictionary = {
        "KEY_UNITS": configuration.units
    };
    console.log('Configuration window returned: ' + configuration.units);
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