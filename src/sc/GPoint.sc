GPoint : UGen {
	*gr { arg numSegments = 32;
		^this.multiNew(
			'audio', 
			numSegments
		);
	}
}

