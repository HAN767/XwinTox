also oop definitions

object subclass c-PBMessage
	cell: .I1
	cell: .I2
	cell: .I3
	cell: .I4

	c-string obj: .S1
	c-string obj: .S2
	c-string obj: .S3
	c-string obj: .S4

	: init { 2:this -- }
		." Initialising a new c-PBMessage" cr
		this --> .S1 --> clr-buf ( it's important to make sure these aren't )
		this --> .S2 --> clr-buf ( internally set to some ridiculous value, as )
		this --> .S3 --> clr-buf ( such a condition can lead to bus faults. )
		this --> .S4 --> clr-buf
	;
end-class
