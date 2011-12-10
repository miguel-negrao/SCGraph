GServer : Server {
	classvar <>graphics;
	classvar <>program;
	classvar <default;

	var verbosity = 0;

	init { arg argName, argAddr, argOptions, argClientID;
		argOptions = argOptions ? GServerOptions.new;
		if (argAddr.isNil, { argAddr = NetAddr("127.0.0.1", 37291) });		
		super.init(argName, argAddr, argOptions, argClientID);
	}

	*initClass {
		Platform.switch(\windows, {
			program = "scgraph.exe";
		}, {
			program = "scgraph";
		});
		default = GServer.new(\graphics, NetAddr("127.0.0.1", 37291));
	}

	// same as in Server.sc, but without SHM
	boot { arg startAliveThread=true, recover=false;
		var resp;
		if (serverRunning, { "server already running".inform; ^this });
		if (serverBooting, { "server already booting".inform; ^this });

		serverBooting = true;
		if(startAliveThread, { this.startAliveThread });
		if(recover) { this.newNodeAllocators } { this.newAllocators };
		bootNotifyFirst = true;
		this.doWhenBooted({
			serverBooting = false;
			if (sendQuit.isNil) {
				sendQuit = not(this.inProcess) and: {this.isLocal};
			};
			
			/*
			if (this.inProcess) {
				serverInterface = ServerShmInterface(thisProcess.pid);
			} {
				if (isLocal) {
					serverInterface = ServerShmInterface(addr.port);
				}
			};
			*/
			this.initTree;
			(volume.volume != 0.0).if({
				volume.play;
				});
		});
		if (remoteControlled.not, {
			"You will have to manually boot remote server.".inform;
		},{
			this.bootServerApp;
		});
	}

	verbosity { arg verbosityLevel = 0;
		this.options.verbosity_(verbosityLevel);
		this.sendMessage("/verbose", verbosityLevel)
	}

	controlRate { arg controlRate = 100;
		this.options.controlRate_(controlRate);
		this.sendMessage("/controlRate", controlRate)
	}

	graphicsRate { arg graphicsRate = 0;
		this.options.graphicsRate_(graphicsRate);
		this.sendMessage("/graphicsRate", graphicsRate)
	}

	bootServerApp {
		pid = (options.pathsAsExports ++ 
			program ++ options.asOptionsString(addr.port)).unixCmd;
		("booting " ++ addr.port.asString).inform;
	}
}

GServerOptions : ServerOptions
{
	var <protocol = \udp;

	var <>controlRate;
	var <>graphicsRate;
	var <>realtimePriority = 0;

	var <>synthDefPath;
	var <>texturePath;
	var <>shaderPath;
	var <>pluginPath;
	var <>scPluginPath;

	var <>persistentSynth;

	var <>verbosity = 0;
	var <>zeroConf = false; // Whether server publishes port to Bonjour, etc.

	var <>restrictedPath;

	var <>initialNodeID = 1000;
	var <>remoteControlVolume = false;

	pathsAsExports {
		var o = "";

		[synthDefPath, "SCGRAPH_SYNTHDEF_PATH",
			texturePath, "SCGRAPH_TEXTURE_PATH",
			shaderPath, "SCGRAPH_SHADER_PATH",
			pluginPath, "SCGRAPH_PLUGIN_PATH",
			scPluginPath, "SCGRAPH_SC_PLUGIN_PATH"].pairsDo{
				|path,envVar|
				if(path.notNil, {
					if(File.exists(path), {
						o = o ++ " " ++ envVar ++ "=\"" ++ path ++ "\"";
					}, {
						warn("Path doesn't exist: " ++ path);
					});
				});
			};

		if(o == "", {
			^o 
		},{
			^("export " ++ o ++ ";")
		});
	}
	
	asOptionsString { | port = 37291 |
		var o;
		// just use UDP anyways
		o = " -u " ++ port;

		if (numControlBusChannels != 4096, {
			o = o ++ " -c " ++ numControlBusChannels;
		});
		if (verbosity != 0, {
			o = o ++ " -" ++ (verbosity.collect{ "v" }.join);
		});
		if (controlRate.notNil, {
			o = o ++ " -f " ++ controlRate;
		});
		if (graphicsRate.notNil, {
			o = o ++ " -r " ++ graphicsRate;
		});
		if (realtimePriority != 0, {
			o = o ++ " -p " ++ realtimePriority;
		});
		if (persistentSynth.notNil, {
			if(File.exists(persistentSynth), {
				o = o ++ " -s " ++ persistentSynth;
			}, {
				warn("File doesn't exist: " ++ persistentSynth);
			});
		});

		^o
	}
}
