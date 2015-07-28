The Forth Interface
==============

XwinTox provides a comprehensive Forth plugin interface.

An example of its use is below:

```forth
object subclass c-SignalHandler
	c-PBMessage obj: .PBMessage

	: init { 2:this }
		this --> super --> init
	;

	: recvSignal { T I1 I2 I3 I4 S1.P S1.L S2.P S2.L S3.P S3.L S4.P S4.L c-addr 2:this }
		." Receive Signal"
		T I1 I2 I3 I4 S1.P S1.L S2.P S2.L S3.P S3.L S4.P S4.L
		this --> .PBMessage --> init
		this --> .PBMessage --> .S1 --> type cr
	;
end-class

c-SignalHandler --> new SHandler

SHandler -->bind-instance-method recvSignal constant signal-received

: register-callback ( c-addr -- )
	signal-received
	swap
	xwfsetcallback
	;

999 register-callback

1 2 3 4 5 6 7 8 xwfsubscribe xwfsubscribe xwfsubscribe xwfsubscribe xwfsubscribe
xwfsubscribe xwfsubscribe xwfsubscribe
```
