.PHONY: clean arch.a

arch.a: 
	@make -C x86_64 arch.a
	@cp x86_64/arch.a ./arch.a
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f *.a
	@make -C x86_64
