var game = new Phaser.Game(800, 600, Phaser.AUTO, 'phaser', { preload: preload, create: create, update: update});

var player;
var otherPlayers = {};

function preload() {
    this.load.image('player', '/assets/knight.png');
    this.load.image('platform_side', 'assets/platform_side.png');
    this.load.image('platform_bottom', 'assets/platform_bottom.png');
}

function create() {

    this.client = new Client();
    this.client.openConnection();

    game.stage.backgroundColor = "#FFFFFF";

    game.physics.startSystem(Phaser.Physics.ARCADE);

    player = this.add.sprite(game.world.centerX,game.world.centerY,'player');
    player.anchor.setTo(0.5,0.5);
    this.physics.arcade.enableBody(player);
    player.body.collideWorldBounds = true;
    this.cursors = this.input.keyboard.createCursorKeys();


    platforms = game.add.group();
    platforms.enableBody = true;
    var bottom = platforms.create(200,100, 'platform_bottom');
    bottom.body.immovable = true;
    bottom = platforms.create(200,468, 'platform_bottom');
    bottom.body.immovable = true;
    var side = platforms.create(200,100, 'platform_side');
    side.body.immovable = true;
    side = platforms.create(570,100, 'platform_side');
    side.body.immovable = true;



}

function update() {
    //Shitty basic movement
    var hSpeed = 0;
    var vSpeed = 0;

    var moveSpeed = 250;

    var hitPlatform = game.physics.arcade.collide(player, platforms);
    
    if(game.input.keyboard.isDown(Phaser.Keyboard.LEFT))
    {
        hSpeed -= 1;
        player.angle = 90;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.RIGHT))
    {
        hSpeed += 1;
        player.angle = -90;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.UP))
    {
        vSpeed -= 1;
        player.angle = -180;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.DOWN))
    {
        vSpeed += 1;
        player.angle = 0;

    }

    //Dash?
    if(game.input.keyboard.isDown(Phaser.Keyboard.SPACEBAR))
    {
       console.error(player.angle);

        switch(player.angle)
        {
            case 0:
                player.position.y+= 5;
                break;
            case -90:
                player.position.x+= 5;
                break;
            case -180:
                player.position.y-= 5;
                break;
            case 90:
                player.position.x-= 5;

        }



    }


    player.body.velocity.x = hSpeed * moveSpeed;
    player.body.velocity.y = vSpeed * moveSpeed;

    if(this.client.connected)
    {
        this.client.sendMessage(player.body.position);
    }
}

function Client() {

}

Client.prototype.openConnection = function() {
    this.ws = new WebSocket("ws://127.0.0.1:9001");
    this.connected = false;
    this.ws.binaryType = "arraybuffer";
    this.ws.onmessage = this.onMessage.bind(this);
    this.ws.onerror = this.displayError.bind(this);
    this.ws.onopen = this.connectionOpen.bind(this);
};

Client.prototype.sendMessage = function(payload) {

    var buffer = new Uint32Array(2);
    buffer[0] = payload.x;
    buffer[1] = payload.y;
    this.ws.send(buffer);
};

Client.prototype.connectionOpen = function() {
    this.connected = true;
    this.sendMessage(player.body.position);
};

Client.prototype.onMessage = function(message) {
    if(typeof message.data == 'string')
    {
        //text message (connections)
        var text = message.data;
        if(text.startsWith('current_connections'))
        {
            text = text.slice('current_connections'.length + 1);
            otherPlayers
        }
        else if(text.startsWith('player_id'))
        {
            text = text.slice('player_id'.length + 1);
            player.id = text;
        }
        else if(text.startsWith('new_connection'))
        {
            text = text.slice('new_connection'.length + 1);
            console.log(text);
        }
    }
    else
    {
        //binary message (position updates)
        var xBuffer = new Int32Array(message.data, 0, 1);
        var yBuffer = new Int32Array(message.data, 4, 1);
    }
};

Client.prototype.displayError = function(err) {
    console.log('Websocketerror: ' + err);
};