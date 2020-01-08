#include <event2/buffer.h>
#include <string.h>

int main()
{
    // struct evbuffer do not provide functions to schedule the IO or trigger the IO
    // that is what bufferevents do
    struct evbuffer *evbuf, *src, *dst;
    
    // Creating or freeing an evbuffer
    evbuf = evbuffer_new();
    evbuffer_free(evbuf);

    // thread-safety
    evbuffer_enable_locking(evbuf, NULL);
    evbuffer_lock(evbuf);
    evbuffer_unlock(evbuf);

    // inspecting an evbuffer
    size_t dataSize = evbuffer_get_length(evbuf);

    // adding data to the end of an evbuffer
    char buf[] = "hello libevent";
    evbuffer_add(evbuf, buf, sizeof(buf)); // just add, don't move
    evbuffer_add_printf(evbuf, "hello %s", "libevent");
    evbuffer_add_buffer(dst, src); // moves all data from src to the end of dst.
    
    // adding data to the front of an evbuffer
    evbuffer_prepend(evbuf, buf, sizeof(buf));  // just like evbuffer_add
    evbuffer_prepend_buffer(dst, src);          // just like evbuffer_add_buffer

    // rearranging the internal layout of an evbuffer 
    /*
     * As the contents of an evbuffer can be stored in multiple different memory blocks, it cannot be accessed directly. Instead, 
     * evbuffer_pullup() can be used to force a specified number of bytes to be contiguous. 
     * This will cause memory reallocation and memory copies if the data is split across multiple blocks. 
     * It is more efficient, however, to use evbuffer_peek() if you don't require that the memory to be contiguous.
     */
    char *mem = evbuffer_pullup(evbuf, -1); // "linearizes" the first size bytes of buf,
    mem[0] = 'h';
    mem[1] = 'e';
    mem[2] = 'l';
    mem[3] = 'l';
    mem[4] = 'o';
    mem[5] = '\0';

    // removing data from an evbuffer
    evbuffer_drain(evbuf, dataSize); // just remove 
    evbuffer_remove(evbuf, buf, sizeof(buf)); // remove and copy
    evbuffer_remove_buffer(src, dst, dataSize);

    // copying data out from an evbuffer, don't change ths src buffer
    evbuffer_copyout(evbuf, buf, sizeof(buf));

    // to do 
    
}