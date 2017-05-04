$(document).ready(function(){
  var ticking = new Audio('https://s3-us-west-2.amazonaws.com/s.cdpn.io/231853/174721__drminky__watch-tick.wav');
  var breakDing = new Audio('https://s3-us-west-2.amazonaws.com/s.cdpn.io/231853/Elevator_Ding-SoundBible.com-685385892.mp3');
  var workDing = new Audio('https://s3-us-west-2.amazonaws.com/s.cdpn.io/231853/Air_Plane_Ding-SoundBible.com-496729130.mp3');
  var timerID;
  var line = 0;
  var minutes;
  var seconds;
  var breakTime = 300;
  var workTime = 1500;
  var running = false;
  var onBreak = false;
  var startTime;
  var tickCheck;
  //empty the timer circle on load
  document.getElementById('circle-fill').setAttributeNS(null, 'stroke-dasharray', "0 100");
  $('#break1').on('click',function() {
    if (!running) 
    {
      (breakTime) -= 60;
      if ((breakTime) < 60) 
      {
          (breakTime) = 60;
      }      
    }
    $('.time1').text((breakTime / 60) + " min");
  });
  $('#break2').on('click',function() {
    if (!running) 
    {
      (breakTime) += 60;
      if ((breakTime) > 50*60) 
      {
          (breakTime) = 50*60;
      }      
    }
    $('.time1').text((breakTime / 60) + " min");
  });  
  $('#break3').on('click',function() {
    if (!running) 
    {
      (workTime) -= 60;
      if ((workTime) < 60) 
      {
          (workTime) = 60; 
      }
      minutes=workTime/60;
      seconds=0;
    }
    $('.time2').text((workTime / 60) + " min");
    $('#countdown').text(workTime/60 + ":00");
 }); 
 $('#break4').on('click',function() {
    if (!running) 
    {
      (workTime) += 60;
      if ((workTime) >50*60) 
      {
        (workTime) = 50*60;
      }      
      minutes=workTime/60;
      seconds=0;
    }
    $('.time2').text((workTime / 60) + " min");
    $('#countdown').text(workTime/60 + ":00");
 });
  //set the default times display in minutes
  $('.time2').text((workTime / 60) + " min");
  $('.time1').text((breakTime / 60) + " min");
  $('#countdown').text(workTime/60 + ":00");
  //This function controls the clock display and ticking
  function countDown(num)
  {
    seconds -= num;
    //play a tick if the second will change
    if (Math.floor(seconds) !== tickCheck  && !onBreak)
    {
      ticking.play();
      tickCheck = Math.floor(seconds);
    }
    //roll over to the next minute
    if(seconds <= 0)
    {
      seconds = 59.9;
      minutes -= 1;
    }
    //prevent minutes from displaying -1
    if (minutes < 0)
    {
      minutes = 0;
    }
    //change the countdown text
    $('#countdown').text(minutes + ":" + ("0" + Math.floor(seconds)).slice(-2));
  }
  //This is the primary timing function that is called in setInterval()
  function timer()
  {
    var currentTime = new Date().getTime();
    //Time to work. Fill the circle.
    if (line < 100 && onBreak === false) 
    {
      line += (((currentTime - startTime) / 1000) / workTime) * 100; 
      countDown((currentTime - startTime)/1000);
      startTime = currentTime;
      document.getElementById('circle-fill').setAttributeNS(null, 'stroke-dasharray', line + " 100");
    //When break is done, start work again
    } 
    else if (line === 0 && onBreak === true) 
    {
      $('.type').text("Busy..");
      workDing.play();
      onBreak = false;
      $('#countdown').text(workTime/60 + ":00");
      minutes = workTime/60;
      seconds = 0;
    //if the circle is full, work time is over, set up break time  
    } 
    else if (line >= 100 || onBreak === true) 
    {
      //If this is the first iteration for break, play the break ding and set sliders, countdown, etc
      if (onBreak === false) 
      {
        $('.type').text("Break!!!");
        breakDing.play();
        $('#countdown').text(breakTime/60 + ":00");
        minutes = breakTime/60;
        seconds = 0;
        onBreak = true;
      }
      //Start the break countdown
      line -= (((currentTime - startTime) / 1000) / breakTime) * 100;
      countDown((currentTime - startTime)/1000);
      startTime = currentTime;
      //Keep the circle from underfilling, creates a visual glitch
      if (line < 0) 
      {
        line = 0;
      }
      document.getElementById('circle-fill').setAttributeNS(null, 'stroke-dasharray', line + " 100");
    }
  }
  //functions for the start, pause, and reset buttons
  $('#start-button').click(function() {
    if (!running) 
    {  
      $('#countdown').text(workTime/60 + ":00");
      startTime = new Date().getTime();
      timerID = setInterval(timer, 50);
      running = true;
    }
    if(!onBreak)
    {
      $('.type').text("Busy..");
    }
    $('.type').addClass("animated bounce");
  });
  $('#pause-button').click(function() {
    running = false;
    clearInterval(timerID);
    $('.type').removeClass("animated bounce");
  });
  $('#reset-button').click(function() {
    running = false;
    onBreak = false;
    $('.type').text("");
    clearInterval(timerID);
    line = 0;
    minutes = workTime/60;
    seconds = 0;
    $('#countdown').text(workTime/60 + ":00");
    $('#circle-fill').css('transition', 'stroke-dasharray 1.5s ease-out');
    document.getElementById('circle-fill').setAttributeNS(null, 'stroke-dasharray', "0 100");
  });
});