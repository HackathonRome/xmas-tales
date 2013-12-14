

function AmbientLightManager() {

    this.message = document.querySelector('.message p');
    this.ambient = document.querySelector('.ambient');
    this.pinkBauble = document.querySelector('.pink.bauble');
    this.redBauble = document.querySelector('.red.bauble');
    this.blueBauble = document.querySelector('.blue.bauble');
    this.yellowBauble = document.querySelector('.yellow.bauble');
    this.greenBauble = document.querySelector('.green.bauble');

//    this.connection = new WebSocket('ws://192.168.1.131:5555');

    var _self = this;
    var offAlreadySent = false;
    var onAlreadySent = false;

    _self.turnTheLightsOn();

    window.ondevicelight = function(deviceLightEvent) {
        console.log(deviceLightEvent);

        if (deviceLightEvent.value >= 1000) {
            _self.hideMessage();
            _self.setSnow();

            if (offAlreadySent === false) {
                _self.sendOffPacket();
                offAlreadySent = true;
                onAlreadySent = false;
            }
        }
        else if (deviceLightEvent.value >= 100) {
            _self.hideMessage();
            _self.setMorning();

            if (offAlreadySent === false) {
                _self.sendOffPacket();
                offAlreadySent = true;
                onAlreadySent = false;
            }
        }
        else if (deviceLightEvent.value >= 10) {
            _self.hideMessage();
            _self.setAfternoon();

            if (offAlreadySent === false) {
                _self.sendOffPacket();
                offAlreadySent = true;
                onAlreadySent = false;
            }
        }
        else {
            _self.showMessage();
            _self.setNight();

            if (onAlreadySent === false) {
                _self.sendOnPacket();
                onAlreadySent = true;
                offAlreadySent = false;
            }
        }

    };
}

AmbientLightManager.prototype = {
    hideMessage: function() {
        console.log('hideMessage');

        var _self = this;

        _self.message.style.visibility = 'hidden';
    },
    showMessage: function() {
        console.log('showMessage');

        var _self = this;

        _self.message.style.visibility = 'visible';
    },
    setSnow: function() {
        console.log('setSnow');

        var _self = this;

        _self.ambient.classList.remove('night');
        _self.ambient.classList.remove('afternoon');
        _self.ambient.classList.remove('morning');
        _self.ambient.classList.add('snow');
    },
    setMorning: function() {
        console.log('setMorning');

        var _self = this;

        _self.ambient.classList.remove('night');
        _self.ambient.classList.remove('afternoon');
        _self.ambient.classList.remove('snow');
        _self.ambient.classList.add('morning');
    },
    setAfternoon: function() {
        console.log('setAfternoon');

        var _self = this;

        _self.ambient.classList.remove('night');
        _self.ambient.classList.remove('morning');
        _self.ambient.classList.remove('snow');
        _self.ambient.classList.add('afternoon');
    },
    setNight: function() {
        console.log('setNight');

        var _self = this;

        _self.ambient.classList.remove('afternoon');
        _self.ambient.classList.remove('morning');
        _self.ambient.classList.remove('snow');
        _self.ambient.classList.add('night');
    },
    turnTheLightsOn: function() {
        console.log('turnTheLightsOn');

        var _self = this;

        setInterval(function() {
            _self.pinkBauble.classList.add('on');
            setTimeout(function() {
                _self.pinkBauble.classList.remove('on');
            }, Math.random() * 8000);
        }, Math.random() * 11000);

        setInterval(function() {
            _self.redBauble.classList.add('on');
            setTimeout(function() {
                _self.redBauble.classList.remove('on');
            }, Math.random() * 8000);
        }, Math.random() * 11000);

        setInterval(function() {
            _self.blueBauble.classList.add('on');
            setTimeout(function() {
                _self.blueBauble.classList.remove('on');
            }, Math.random() * 8000);
        }, Math.random() * 9000);

        setInterval(function() {
            _self.yellowBauble.classList.add('on');
            setTimeout(function() {
                _self.yellowBauble.classList.remove('on');
            }, Math.random() * 7000);
        }, Math.random() * 8000);

        setInterval(function() {
            _self.greenBauble.classList.add('on');
            setTimeout(function() {
                _self.greenBauble.classList.remove('on');
            }, Math.random() * 6000);
        }, Math.random() * 6000);
    },
    sendOnPacket: function() {
        console.warn('sendOnPacket');

        var connection = new WebSocket('ws://192.168.1.131:5555');
        connection.onopen = function() {
            this.connection.send('on');
            connection.close();
        };
    },
    sendOffPacket: function() {
        console.warn('sendOffPacket');

        var connection = new WebSocket('ws://192.168.1.131:5555');

        connection.onopen = function() {
            this.connection.send('off');
            connection.close();
        };
    }

}
