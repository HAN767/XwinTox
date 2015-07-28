also oop definitions

\ helpers for -->bind-xt-for-method

: (exec-instance-method)         ( xt-thisword )
     dup 2@                       ( xt-thisword cls inst )
     rot 2 cells + @              ( cls inst xt-clsMeth )
     execute
;

: (bind-instance-method) ( inst cls xt-clsMeth -- xt-instMeth )

     \ create a word to hold the 'inst cls xt', and run the method
     s" create _im_" evaluate    ( inst cls xt-clsMeth )
     -rot                             ( xt-clsMeth inst cls )
     , , ,                            ( )
     last-word                        ( xt-instMeth )
     does>
         \ ." instance method runner started" cr
         (exec-instance-method)
;

\ binds an instance method to an xt, so whenever the
\ xt is executed, the object signature ( inst cls ) is
\ pushed onto the stack, and the method executed

: -->bind-instance-method                ( inst cls -- xt )

     state @ if
         \ compiling
         parse-method                     ( inst cls addr u )
         postpone lookup-method           ( inst cls xt-clsMeth )
         postpone (bind-instance-method)  ( xt-instMeth )
     else
         \ interpreting
         parse-word                       ( inst cls addr u )
         lookup-method                    ( inst cls xt-clsMeth )
         (bind-instance-method)           ( xt-instMeth )
     then

; immediate

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

