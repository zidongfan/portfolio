import java.util.HashMap;

/**
 * This class is the local cache. It should basically just have a HashMap<DNSQuestion, DNSRecord> in it
 * You can just store the first answer for any question in the cache (a response for google.com might return 10 IP addresses, just store the first one)
 * This class should have methods for querying and inserting records into the cache
 * When you look up an entry, if it is too old (its TTL has expired), remove it and return "not found."
 */

public class DNSCache {
    private static HashMap<DNSQuestion, DNSRecord> hashMap = new HashMap<>();

    DNSRecord query(DNSQuestion dnsQuestion) {
        if (hashMap.containsKey(dnsQuestion) && hashMap.get(dnsQuestion).isExpired()) {
            return hashMap.get(dnsQuestion);
        } else {
            hashMap.remove(dnsQuestion);
            return null;
        }
    }

    void insert(DNSQuestion dnsQuestion, DNSRecord dnsRecord) {
        hashMap.put(dnsQuestion, dnsRecord);
    }
}
