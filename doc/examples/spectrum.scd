g = GServer.default;
g.boot;

(
SynthDef (\Spectrum, {
	var bars;

	bars = Array.fill 
	(
		17, 
		{
			arg i; 
			var freq, amp, height;

			freq =  1/((10*i)+1);
			amp = GFreqAmp.kr(port: 1,freq: freq);
			height = 5 * (1 + (100 * Lag.kr (amp, 0.5))).log;

		  	GTranslate.gr 
			(
				GRectangle.gr 
				(
					width: 1, 
					height: height
				), 
				[2 * i - 17, 0, 0]
			);
		}
	).sum;

	GGLRenderer.gr (bars, clear_mode: 1, clear_color: [0, 0.2, 0, 0.2]);
}).play(g)
)
