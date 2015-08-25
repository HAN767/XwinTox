/ObjectTemplate <<  % All objects have these fields.
    /parentDict       null  % link to my parent dict; stops at null.
    /workingDict      null  % temporary storage for the current object
>> def

/ClassTemplate <<   % Class objects have these fields in addition:
    /instanceVars     null  % this class's inst vars
    /className        null  % name of the class (as a keyword)
>> def

% Create a sub-class of the given class.
/@class
{ % classname superclass instvars => - (newclass on dict stack)
dictbegin
    ObjectTemplate { def } forall
    ClassTemplate { def } forall

    /instanceVars exch def
    /parentDict exch def
    /className exch def
} def

/@endclass
{
    className dictend
} def

/XWObject null [] 
@class
    /new 
    { % class => instance
        dictbegin
        instanceVars { def } forall % we ought to recurse down through the
                                    % parent dictionaries and def their ivars
        ObjectTemplate { def } forall
        dictend
        /parentDict currentdict put
        "New XWObject\n" print
    } def
@endclass def

/s->
{ % procname => parentdict
    parentDict exch get exec % we need to do this without popping ourself
                             % like in the regular -> we ought to recurse
} def

/->
{ % instance procedure => (proc executed for instance)
    exch begin
    % we should test if the name can be resolved in the CURRENT dictionary
    % if it can't be, we should open its parent dict and try there, and
    % recursively open parent dicts until we find the method, before finally
    % throwing an error
    cvx exec
    end pop
} def


/TestObj XWObject
<<
    /TestValue 0
    /AnotherValue 2
>>
@class
    /new
    {
        /new s->
        "New TestObj\n" print
    } def
@endclass def
