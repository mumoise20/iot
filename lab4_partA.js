'use strict';
var Gpio = require('onoff').Gpio;
    const sensor = new Gpio(17, 'in', 'both');
    const led1 = new Gpio(4, 'out');
    const led2 = new Gpio(27, 'out');
    var value = sensor.readSync();
    var count = 0;
        
        const timer = setInterval(() => {
            count+=1;
            if (value==1)
    {
            if (count%2){
        console.log('""PIR: there is some one!""') ;
        led1.writeSync(value);
        console.log("Changed LED1 state to: " + value);
        led2.writeSync(0)
        console.log("Changed LED2 state to: " + 0); }      
        if (!(count%2)){
        led2.writeSync(value)
        console.log("Changed LED2 state to: " + value);
        led1.writeSync(0)
        console.log("Changed LED1 state to: " + 0);}
       
    }
    value = sensor.readSync();

        if (value==0)
    {
        console.log("No one!!");
        led2.writeSync(0);
        led1.writeSync(0);
    }

},100);

function exit(err) {
    if (err) console.log('An error occurred: ' + err);
    sensor.unexport();
    clearInterval(timer);
    console.log('Bye, bye!')
    process.exit();
}
process.on('SIGINT', exit);