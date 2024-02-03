import java.util.HashMap;
import java.util.Map;
public class DNSCache {

    private Map<DNSQuestion, DNSRecord> cache;

    public DNSCache() {
        cache = new HashMap<>();
    }

    //This class should have methods for querying and inserting records into the cache.
    public void insert(DNSQuestion question, DNSRecord record) {
        cache.put(question, record);
    }
//    public void insert(DNSQuestion question, DNSRecord answer){
//        if(!cache.containsKey(question)){
//            cache.put(question.answer);
//        }
//    }
    public DNSRecord query(DNSQuestion question){
        DNSRecord record = cache.get(question);
        if(record != null && !record.isExpired()){
            return record;
        } else {
            // When you look up an entry, if it is too old (its TTL has expired), remove it and return "not found."
            cache.remove(question);
            return null;

        }
    }

}
