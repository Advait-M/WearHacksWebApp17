var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function scriptReq() {
  var url = "https://popping-inferno-3368.firebaseio.com/.json";

  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);
      console.log("Read: " + json.Child);

      var dictionary = {
        "Script": json.Child
      };

      Pebble.sendAppMessage(dictionary);
    }
  );
}

function posReq() {
  var url = "https://popping-inferno-3368.firebaseio.com/.json";

  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);
      console.log("Read: " + json.Pos);

      var dictionary = {
        "Position": json.Pos
      };

      Pebble.sendAppMessage(dictionary);
    }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");
    scriptReq();
    posReq();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    posReq();
  }
);
