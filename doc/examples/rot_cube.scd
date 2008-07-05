a.sendMsg("/g_new", -1, 0, 0);


(
SynthDef (\Test,
{
	var light, geometry, renderer, rot_axis, spot_direction;

	rot_axis = [SinOsc.kr(0.361), SinOsc.kr(0.231, phase: 1.7), SinOsc.kr(0.23456)];
	spot_direction = [-10,-5,-10];


	light = GLight.ar 
	(
		spot_direction: spot_direction, 
	  // specular_color: Color.rand.asArray, 
		diffuse_color: Color.rand.asArray, 
		position: (-1.0 * spot_direction) ++ 1
	);

  	//geometry = GRotate.ar(GCube.ar, angle:  Lag.kr(0.3 * WhiteNoise.kr, 5));
  	geometry = Array.fill(15, 
	{
		arg i; 
		GRotate.ar
		(
			GTranslate.ar
			(
				GCube.ar, 
				[3*(i-8), 0, 0]
			),
			axis: rot_axis, angle: 3.131 * LFSaw.kr(0.1 * (i - 8))
		)
	}).sum;

	renderer = GGLRenderer.ar(light + geometry, lighting:1, clear_mode:0, clear_color: [0,0,0,0.04]);
}).play(s)
)

(
SynthDef (\Test,
{
	GGLRenderer.ar (GTranslate.ar (GCube.ar, [0, SinOsc.kr(1), 0]));
}).play(s)
)