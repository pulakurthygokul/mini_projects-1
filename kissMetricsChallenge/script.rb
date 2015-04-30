require 'tokyocabinet'
include TokyoCabinet

hdb = HDB::new
if !hdb.open('people.db', HDB::OREADER) 
    errcode = hdb.ecode()
    printf(hdb.errmsg(errcode)+"\n")
end
