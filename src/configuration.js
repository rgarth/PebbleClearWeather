Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  var units = localStorage.getItem('units');
  if (typeof units == 'undefined') units = "us";
  var timeChoice = localStorage.getItem('time');
  if (typeof timeChoice == 'undefined') timeChoice = "12";
  var URL = 'http://rgarth.github.io/PebbleClearWeather/configuration.html?units=' + 
      units + '&time=' + timeChoice;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL(URL);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var dictionary = {
      "KEY_UNITS": configuration.units,
      "KEY_TIMESWITCH": parseInt(configuration.time)
    };
    localStorage.setItem('units', configuration.units);
    localStorage.setItem('time', configuration.time);
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
