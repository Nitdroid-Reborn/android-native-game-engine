mutt = nill;

tekst = "aaa";
update = function()
    io.write(tekst);
end

func = function(obj)
        obj:bar();
        foo:getboo():hello();
        x = foo:new();
        return x;
end

set_mutt = function(obj)
io.write ("\n\t(Lua)set_mutt Called!\n\t{")
	mutt = obj;
io.write ("\n\t}\n\t(Lua)set_mutt End!")
end

try_mutt = function()
io.write ("\n\t(Lua)try_mutt Called!\n\t{")
	mutt:bar();
io.write ("\n\t}\n\t(Lua)try_mutt End!")
end
