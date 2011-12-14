GTube : UGen {
	*gr {
		arg size = 1.0, numSegments = 32;
		^this.multiNew('audio', size, numSegments);
	}
}

