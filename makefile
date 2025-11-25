all:
	g++ cse4001_sync.cpp -o cse4001_sync
	g++ reader_writer_no_starve.cpp -lpthread -o reader_writer_no_starve -lm
	g++ writer_priority.cpp -lpthread -o writer_priority -lm
	g++ dining_philosophers1.cpp -lpthread -o dining_philosophers1 -lm
	g++ dining_philosophers2.cpp -lpthread -o dining_philosophers2 -lm

clean:
	rm cse4001_sync
	rm reader_writer_no_starve
	rm writer_priority
	rm dining_philosophers1
	rm dining_philosophers2