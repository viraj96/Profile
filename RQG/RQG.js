$(document).ready(function() { 
  function displayquote()
  {
    quotes=[];
    authors=[];
    quotes[0]="The best preparation for tomorrow is doing your best today";
    authors[0]="H. Jackson Brown, Jr.";
    quotes[1]="Start by doing what's necessary; then do what's possible; and suddenly you are doing the impossible";
    authors[1]="Fancis of Assisi";
    quotes[2]="The best and most beautiful things in the world cannot be seen or even touched - they must be felt with the heart";
    authors[2]="Hellen Keller";
    quotes[3]="I believe in living today. Not in yesterday, nor in tomorrow";
    authors[3]="Loretta Young";
    quotes[4]="I can't change the direction of the wind, but I can adjuct my sails to always reach my destination";
    authors[4]="Jimmy Dean";
    quotes[5]="Nothing is impossible, the word itself says 'I'm possible!";
    authors[5]="Audrey Hepburn";
    quotes[6]="Perfection is not attainable, but if we chase perfection we can catch excellence";
    authors[6]="Vince Lombardi";
    quotes[7]="Try to be a rainbow in someone's cloud";
    authors[7]="Maya Angelou";
    quotes[8]="Believe you can and you're halfway there";
    authors[8]="Theodore Roosevelt";
    quotes[9]="As we express our gratitude, we must never forget thet the highest appreciation is not to utter words, but to live by them";
    authors[9]="John F. Kennedy";
    quotes[10]="We know what we are, but know not what we may be";
    authors[10]="William Shakespeare";
    quotes[11]="A hero is someone who has given his or her life to something bigger than oneself";
    authors[11]="Joseph Campbell";
    quotes[12]="We must let go of the life we have planned, so as to accept the one that is waiting for us";
    authors[12]="Joseph Campbell";
    quotes[13]="If opportunity doesn't knock build a door";
    authors[13]="Milton Berle";
    quotes[14]="Put your heart, mind, and soul into even your smallest acts. This is the secret of success";
    authors[14]="Swami Sivananda";
    quotes[15]="Change your thoughts and you change your world";
    authors[15]="Norman Vincent Peale";
    quotes[16]="Health is the greatest gift, contentment the greatest wealth, faithfulness the best relationship.";
    authors[16]="Buddha";
    quotes[17]="There are two ways of spreading light: to be the candle or the mirror that reflects it";
    authors[17]="Edith Wharton";
    quotes[18]="What you do today can improve all your tomorrows";
    authors[18]="Ralph Marston";
    quotes[19]="Don't watch the clock; do what it does. Keep going";
    authors[19]="Sam Levenson";
    var length = quotes.length;
    var index = Math.round(Math.random()*(length-1));
    $(".quote").text("\""+quotes[index]+" \"");
    $(".author").text("-- "+authors[index]);
    } 
    displayquote();
    $(".button").on('click',function() {
        displayquote();
    });
});