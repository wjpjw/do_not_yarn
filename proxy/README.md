# proxy

It's a 3-layer component-based data access proxy server.

Session layer: manage M(M<=10000) sess objects; zmq based msg exchange between proxy and client 

Cache layer:
1. in-memory kv store
2. a cache updater

Core layer:
1. a global task queue (thread-safe)
2. a logger 
3. N worker threads (dequeue tasks; one sess )
4. a conn pool that maintains N db connections (thread-safe)

N = cores*2+1, by default. 
