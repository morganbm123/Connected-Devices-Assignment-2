

var address = '128.122.151.172';
var userName = 'Gp8ZVxoo5J8p5BbQETPgrWPNN5jLDLKLvJWYtUpP';
var url;
var lightNumber = 1;
var hueBulbState = false;
var hueSlider;
var satSlider;
var brightnessSlider;
var stateButton;

var prevHueValue = 0;
var prevSatValue = 0;
var prevBriValue = 0;





function setup(){
  createCanvas(window.innerWidth, window.innerHeight);
  background(57, 79, 81);
  url = 'http://' + address + '/api/' + userName;


  fill(0);
  textSize(15);
  stateButton = createButton('On');
  stateButton.mousePressed(checkLightMode);
  stateButton.position(window.innerWidth / 2, window.innerHeight / 2);

   hueSlider = createSlider(0, 65535, 14922, 100);
  hueSlider.position(window.innerWidth / 2, window.innerHeight / 2 - 30);
  text('Hue', window.innerWidth / 2 - 80, window.innerHeight / 2 - 30);

   satSlider = createSlider(0, 254, 144, 1);
  satSlider.position(window.innerWidth / 2, window.innerHeight / 2 - 60);
  text('Saturation', window.innerWidth / 2 - 80, window.innerHeight / 2 - 60);


   brightnessSlider = createSlider(0, 254, 254, 1);
  brightnessSlider.position(window.innerWidth / 2, window.innerHeight / 2 - 90);
  text('Brightness', window.innerWidth / 2 - 80, window.innerHeight / 2 - 90);

}

function draw(){

 colorMode(HSB);
 fill(map(hueSlider.value(),0,65535,0,360,true),map(satSlider.value(),0,254,0,100,true),map(brightnessSlider.value(),0,360,0,100,true),1);
 ellipse((window.innerWidth / 2 - 90) + brightnessSlider.size().width,window.innerHeight/ 2 - brightnessSlider.size().height/2 - 120, 50,50);


//only send the values of the slider to setLight if they have changed to avoid overrunning the hub
 if ((hueSlider.value() != prevHueValue) || (satSlider.value() != prevSatValue) || (brightnessSlider.value() != prevBriValue)){
   setLight();
   prevHueValue = hueSlider.value();
   prevBriValue = brightnessSlider.value();
   prevSatValue = satSlider.value();

 }
}
//send PUT requests to the hub
function setLight() {
  var path = url + '/lights/' + lightNumber + '/state';
  var content = {'hue': hueSlider.value(), 'brightness': brightnessSlider.value(), 'sat': satSlider.value()};				 // convert JSON obj to string
  httpDo( path, 'PUT', content, 'text', getResponse); //HTTP PUT the change
}

//Get the state of the lights depending on the user input
function lightMode(){
  var path = url + '/lights'
  httpDO(path, 'GET', checkLightMode )

}
//check the lights state
function checkLightMode(){

  var content = {'on': !hueBulbState};
  var path = url + '/lights/' + lightNumber + '/state/'
  httpDo(path, 'PUT', content, changeBulbState);

}
//change the title of the button depending on the bulbs next state.
function changeBulbState(dataRecieved){
  var response = JSON.stringify(dataRecieved);
  if (response.includes("success")){
    hueBulbState = !hueBulbState;
    var lightStr = hueBulbState ? "Off" : "On";
    stateButton.html("Turn " + lightStr);
  }
}

function getResponse(response) {
  console.log(response);
}
