also oop definitions

object subclass c-PBMessage
	c-cell obj: .T

	c-cell obj: .I1
	c-cell obj: .I2
	c-cell obj: .I3
	c-cell obj: .I4

	c-string obj: .S1
	c-string obj: .S2
	c-string obj: .S3
	c-string obj: .S4

	: init { T I1 I2 I3 I4 S1.P S1.L S2.P S2.L S3.P S3.L S4.P S4.L  2:this -- }
		." Initialising a new c-PBMessage" cr
		T	this --> .T	 --> set
		I1 this --> .I1 --> set
		I2 this --> .I2 --> set
		I3 this --> .I3 --> set
		I4 this --> .I4 --> set
		this --> .S1 --> clr-buf ( it's important to make sure these aren't )
		this --> .S2 --> clr-buf ( internally set to some ridiculous value, as )
		this --> .S3 --> clr-buf ( such a condition can lead to bus faults. )
		this --> .S4 --> clr-buf
		S1.P S1.L this --> .S1 --> set
		S2.P S2.L this --> .S2 --> set
		S3.P S3.L this --> .S3 --> set
		S4.P S4.L this --> .S4 --> set
	;
end-class

: signal-received ( c-addr -- ) \ c-addr is the userdata
	s" Forth says: Signal received" type cr
	;

: register-callback ( c-addr -- )
	['] signal-received
	swap
	xwfsetcallback
	;


\ testing commences below 
15 24 23 87 90
: S1 S" Test One" ;
: S2 S" Test Two" ;
: S3 S" Test Three" ;
: S4 S" Test Four" ;
S1 S2 S3 S4
.s
c-PBMessage --> new Msg


Msg --> .S1 --> type
Msg --> .S2 --> type
Msg --> .S3 --> type
Msg --> .S4 --> type

999 register-callback

1 2 3 4 5 6 7 8 xwfsubscribe xwfsubscribe xwfsubscribe xwfsubscribe xwfsubscribe
xwfsubscribe xwfsubscribe xwfsubscribe
