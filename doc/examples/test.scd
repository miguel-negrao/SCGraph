s = Server.default = GServer.default.boot
s = GServer.default
s.quit

s.freeAll
s.quitAll;
s.status;

a.sendMsg("/status");
a.sendMsg("/foo");
a.sendMsg("/notify", 1);
a.sendMsg(9,"Ugha", 1, 0, 0);
a.sendMsg("/quit");
a.sendMsg("/g_freeAll");
a.sendMsg("/g_freeAll", 1);
n.sendMsg("/g_new", -1, 0, 0);
a.sendMsg("/s_new", "SimpleCube", -1, 0, 0);
a.sendMsg("/s_new", "SimpleSin", -1, 0, 0);
a.sendMsg("/c_set", 0, 0.1);
5.do {a.sendMsg("/s_new", "SimpleView", -1, 1, 0);}
a.sendMsg("/d_free", "Ugh");
a.sendMsg("/d_freeAll");

s.sendMsg("/verbose",0);
s.sendMsg("/verbose",1);
s.sendMsg("/verbose",2);
s.sendMsg("/verbose",3);
s.sendMsg("/verbose",4);
s.sendMsg("/verbose",5);

s.sendMsg("/graphicsRate",25);
s.sendMsg("/controlRate", 100);

s.sendMsgSync;
s.sendMsg(3);


(
	SynthDef (
		\WobblingCube,
		{
			Out.gr (0, GTranslate.gr (GCube.gr (size: SinOsc.kr (freq: 0.5)), [-3, -2, 3]));
		}
	).store;
)

// Tell the server to produce one instance of this Definition
s = Synth.new (\WobblingCube, target: 0, addAction: \addToHead);



(
	SynthDef (
		\SimpleView,
		{
			GGLRenderer.gr (In.gr (0));
		}
	).store;
)

v = Synth.new (\SimpleView, target: 0, addAction: \addToTail);

(
SynthDef (\WobblingCube,
{
        var source;
        source = GCube.gr (size: 1 + SinOsc.kr(freq: 0.5));
        GGLRenderer.gr(source);
}).play;
)