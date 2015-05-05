Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  var units = localStorage.getItem('units');
  if (! units) units = "us";
  var timeChoice = localStorage.getItem('time');
  if (! timeChoice) timeChoice = "12";
  var updateInterval = localStorage.getItem('update');
  if (! updateInterval) updateInterval = 30;
  var URL = 'http://rgarth.github.io/PebbleClearWeather/configuration.html?' + 
      'units=' + units + 
      '&time=' + timeChoice + 
      '&update=' + updateInterval;
  console.log('Configuration window opened. ' + URL);
  Pebble.openURL(URL);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var updateInterval = parseInt(configuration.update) || 30;
    var timeChoice = parseInt(configuration.time) || 12;
    var dictionary = {
      "KEY_UNITS": configuration.units,
      "KEY_TIMESWITCH": timeChoice,
      "KEY_UPDATE_INTERVAL": updateInterval,
    };
    localStorage.setItem('units', configuration.units);
    localStorage.setItem('time', timeChoice);
    localStorage.setItem('update', updateInterval);
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
