/$error 
<<
    /errorname null
>> def

% $error begin
"$error installed..." print

/.error 
{
    $error 
	begin
      /errorname exch def
      errordict begin handleerror end
    end 
	pop pop
} def

".error installed..." print

/errordict
<<
    /handleerror
    {
        "!! Error: " print errorname print " !!\n" print
    }
    /errstackunderflow { /StackUnderflow .error }
    /errtypecheck { /TypeCheck .error }
>> def

"errordict installed...\n" print
