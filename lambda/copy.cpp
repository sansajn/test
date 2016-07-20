int main(int argc, char * argv[])
{
	int x = 20;  // x is local variable
	
	auto c1 = [x](int y){ return x*y > 55;};  // c1 is copy of the
															// closure produced
															// by the lambda
	
	auto c2 = c1;  // c2 is copy of c1
	
	auto c3 = c2;  // c3 is copy of c2
	
	return 0;
}
