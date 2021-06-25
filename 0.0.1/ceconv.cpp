
unsigned long long cec_utf8_to_ucs2be(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size)
{
	unsigned long long i = 0, j = 0;
	unsigned char calc_tmp;
	for (;;)
	{
		if (i >= size)
		{
			break;
		}
		if (load_buffer[i] < 0x80)
		{
			store_buffer[j + 1] = load_buffer[i];
			i++;
			store_buffer[j] = 0x00;
			j += 2;
			continue;
		}
		//0x80 ~ 0xe0   Available bits 11(0x07ff max).
		if (load_buffer[i] >= 0x80 && load_buffer[i] < 0xe0)   //0x80 = 110x xxxx 0xe0 = 1110 xxxx
		{
			unsigned char tmp[2];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x1f; /* tmp[0] <<= 3 */ //No shr and shl. because the registers are 32bits or 64bits,and the char only 8bits!
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp &= 0x1c;
			store_buffer[j] >>= 2;
			store_buffer[j] &= 0x07; //for the safety.
			j++;

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp &= 0x03;
			store_buffer[j] <<= 6;
			store_buffer[j] &= 0xc0;   //for the safety.
			store_buffer[j] += tmp[1];
			j++;
			continue;
		}
		//0xe0 ~ 0xf0   Available bits 16(0xffff max).
		if (load_buffer[i] >= 0xe0 && load_buffer[i] < 0xf0)   //0xe0 = 1110 xxxx 0xf0 = 1111 0xxx
		{
			unsigned char tmp[3];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x0f; /* tmp[0] <<= 4 */ //No shr and shl. because the registers are 32bits or 64bits,and the char only 8bits!
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 4;
			store_buffer[j] &= 0xf0; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[1];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j++;
			tmp[1] &= 0x03;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 6;
			
			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			store_buffer[j] += tmp[2];
			j++;
			continue;
		}
		//0xf0 ~ 0xf8   Available bits 21(0x1fffff max).
		if (load_buffer[i] >= 0xf0 && load_buffer[i] < 0xf8)   //0xf0 = 1111 0xxx 0xf8 = 1111 10xx
		{
			unsigned char tmp[4];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x07; 
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 2;
			store_buffer[j] &= 0x1c; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[1];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j++;
			tmp[1] &= 0x0f;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 4;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			calc_tmp = tmp[2];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j++;
			tmp[2] &= 0x03;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 6;

			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			store_buffer[j] += tmp[3];
			j++;
			continue;
		}
		//0xf8 ~ 0xfc   Available bits 26(0x3fffffff max).
		if (load_buffer[i] >= 0xf8 && load_buffer[i] < 0xfc)   //0xf8 = 1111 10xx 0xfc = 1111 110x
		{
			unsigned char tmp[5];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x03;
			store_buffer[j] = tmp[0]; //for the safety.
			store_buffer[j] &= 0x03; //for the safety.
			j++;

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 2;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			calc_tmp = tmp[2];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j++;
			tmp[2] &= 0x0f;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 4;

			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			calc_tmp = tmp[3];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j++;
			tmp[3] &= 0x03;
			store_buffer[j] = tmp[3];
			store_buffer[j] <<= 6;

			tmp[4] = load_buffer[i];
			i++;
			tmp[4] &= 0x3f;
			store_buffer[j] += tmp[4];
			j++;
			continue;
		}
		//over 0xfc   Available bits 31(0x7fffffff max).
		if (load_buffer[i] >= 0xfc)   //0xfc = 1111 110x
		{
			unsigned char tmp[6];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x01;
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 7; //for the safety.
			store_buffer[j] &= 0x80; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			store_buffer[j] += tmp[1];
			j++;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 2;
 
			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			calc_tmp = tmp[3];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j++;
			tmp[3] &= 0x0f;
			store_buffer[j] = tmp[3];
			store_buffer[j] <<= 4;

			tmp[4] = load_buffer[i];
			i++;
			tmp[4] &= 0x3f;
			calc_tmp = tmp[4];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j++;
			tmp[4] &= 0x03;
			store_buffer[j] = tmp[4];
			store_buffer[j] <<= 6;

			tmp[5] = load_buffer[i];
			i++;
			tmp[5] &= 0x3f;
			store_buffer[j] += tmp[5];
			j++;
			continue;
		}
	}
	return j;
}

unsigned long long cec_utf8_to_ucs2le(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size)
{
	unsigned long long i = 0, j = 0;
	unsigned char calc_tmp;
	for (;;)
	{
		if (i >= size)
		{
			break;
		}
		if (load_buffer[i] < 0x80)
		{
			store_buffer[j] = load_buffer[i];
			i++;
			store_buffer[j+1] = 0x00;
			j += 2;
			continue;
		}
		//0x80 ~ 0xe0   Available bits 11(0x07ff max).
		if (load_buffer[i] >= 0x80 && load_buffer[i] < 0xe0)   //0x80 = 110x xxxx 0xe0 = 1110 xxxx
		{
			j++;
			unsigned char tmp[2];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x1f; /* tmp[0] <<= 3 */ //No shr or shl. because the registers are 32bits or 64bits,and the char only 8bits!
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp &= 0x1c;
			store_buffer[j] >>= 2;
			store_buffer[j] &= 0x07; //for the safety.
			j--;

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp &= 0x03;
			store_buffer[j] <<= 6;
			store_buffer[j] &= 0xc0;   //for the safety.
			store_buffer[j] += tmp[1];
			j += 2;
			continue;
		}
		//0xe0 ~ 0xf0   Available bits 16(0xffff max).
		if (load_buffer[i] >= 0xe0 && load_buffer[i] < 0xf0)   //0xe0 = 1110 xxxx 0xf0 = 1111 0xxx
		{
			j++;
			unsigned char tmp[3];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x0f; /* tmp[0] <<= 4 */ //No shr or shl. because the registers are 32bits or 64bits,and the char only 8bits!
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 4;
			store_buffer[j] &= 0xf0; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[1];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j--;
			tmp[1] &= 0x03;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 6;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			store_buffer[j] += tmp[2];
			j += 2;
			continue;
		}
		//0xf0 ~ 0xf8   Available bits 21(0x1fffff max).
		if (load_buffer[i] >= 0xf0 && load_buffer[i] < 0xf8)   //0xf0 = 1111 0xxx 0xf8 = 1111 10xx
		{
			j += 2;
			unsigned char tmp[4];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x07;
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 2;
			store_buffer[j] &= 0x1c; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			calc_tmp = tmp[1];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j--;
			tmp[1] &= 0x0f;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 4;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			calc_tmp = tmp[2];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j--;
			tmp[2] &= 0x03;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 6;

			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			store_buffer[j] += tmp[3];
			j += 3;
			continue;
		}
		//0xf8 ~ 0xfc   Available bits 26(0x3fffffff max).
		if (load_buffer[i] >= 0xf8 && load_buffer[i] < 0xfc)   //0xf8 = 1111 10xx 0xfc = 1111 110x
		{
			j += 3;
			unsigned char tmp[5];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x03;
			store_buffer[j] = tmp[0]; 
			store_buffer[j] &= 0x03; //for the safety.
			j--;

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			store_buffer[j] = tmp[1];
			store_buffer[j] <<= 2;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			calc_tmp = tmp[2];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j--;
			tmp[2] &= 0x0f;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 4;

			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			calc_tmp = tmp[3];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j--;
			tmp[3] &= 0x03;
			store_buffer[j] = tmp[3];
			store_buffer[j] <<= 6;

			tmp[4] = load_buffer[i];
			i++;
			tmp[4] &= 0x3f;
			store_buffer[j] += tmp[4];
			j += 4;
			continue;
		}
		//over 0xfc   Available bits 31(0x7fffffff max).
		if (load_buffer[i] >= 0xfc)   //0xfc = 1111 110x
		{
			j += 3;
			unsigned char tmp[6];
			tmp[0] = load_buffer[i];
			i++;
			tmp[0] &= 0x01;
			calc_tmp = tmp[0];
			store_buffer[j] = calc_tmp <<= 7; 
			store_buffer[j] &= 0x80; //for the safety.

			tmp[1] = load_buffer[i];
			i++;
			tmp[1] &= 0x3f;
			store_buffer[j] += tmp[1];
			j--;

			tmp[2] = load_buffer[i];
			i++;
			tmp[2] &= 0x3f;
			store_buffer[j] = tmp[2];
			store_buffer[j] <<= 2;

			tmp[3] = load_buffer[i];
			i++;
			tmp[3] &= 0x3f;
			calc_tmp = tmp[3];
			store_buffer[j] += (((calc_tmp &= 0x30) >>= 4) &= 0x03);
			j--;
			tmp[3] &= 0x0f;
			store_buffer[j] = tmp[3];
			store_buffer[j] <<= 4;

			tmp[4] = load_buffer[i];
			i++;
			tmp[4] &= 0x3f;
			calc_tmp = tmp[4];
			store_buffer[j] += (((calc_tmp &= 0x3c) >>= 2) &= 0x0f);
			j--;
			tmp[4] &= 0x03;
			store_buffer[j] = tmp[4];
			store_buffer[j] <<= 6;

			tmp[5] = load_buffer[i];
			i++;
			tmp[5] &= 0x3f;
			store_buffer[j] += tmp[5];
			j += 4;
			continue;
		}
	}
	return j;
}

unsigned long long cec_ucs2le_to_utf8(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size)
{
	unsigned short present_char = 0;
	unsigned long long i = 0,j = 0;
	for(;;)
	{
		if(i >= size)
		{
			break;
		}
		present_char = *(unsigned short*)(load_buffer + i);   //The target opcodes should be ... 	
														 /*
                              amd64 ->mov rbx, qword ptr ds:[&i]
                                      mov ax, word ptr ds:[load_buffer + rbx]
                                      mov word ptr ds:[present_char], ax
                                      ...

																	    *"rax/rbx" is a example register for 64bits x86 assembly language(amd64).
														  */
																	
		i += 2;
		unsigned char* present_char_upper8 = (unsigned char*)&present_char + 1;

		if(present_char < 0x80)
		{
			store_buffer[j] = *(unsigned char*)(&present_char);
			j++;
			continue;
		}
		if(present_char >= 0x80 && present_char < 0x800)
		{
			store_buffer[j] = 0xc0;
			unsigned char tmp[2];		
			tmp[0] = *present_char_upper8;
			tmp[0] <<= 2;
			tmp[1] = *(unsigned char*)(&present_char);
			tmp[1] >>= 6;
			tmp[1] &= 0x03;   //for the safety
			store_buffer[j] += tmp[1];
			j++;
			
			store_buffer[j] = 0x80;
			tmp[1] = *(unsigned char*)(&present_char);
			tmp[1] &= 0x3f;
			store_buffer[j] += tmp[1];
			j++;
			continue;
		}
		if(present_char >= 0x800 && present_char < 0x10000)
		{
			store_buffer[j] = 0xe0;
			unsigned char tmp[3];
			tmp[0] = *present_char_upper8;
			tmp[0] >>= 4;
			tmp[0] &= 0x0f;   //for the safety
			store_buffer[j] += tmp[0];
			j++;
			
			store_buffer[j] = 0x80;
			tmp[1] = *present_char_upper8;
			tmp[1] &= 0x0f;
			tmp[1] <<= 2;
			store_buffer[j] += tmp[1];
			tmp[2] = *(unsigned char*)(&present_char);
			tmp[2] >>= 6;
			tmp[2] &= 0x03;   //for the safety
			store_buffer[j] += tmp[2];
			j++;
			
			store_buffer[j] = 0x80;
			tmp[2] = *(unsigned char*)(&present_char);
			tmp[2] &= 0x3f;
			store_buffer[j] += tmp[2];
			j++;
			
			continue;			
		}
		if(present_char >= 0x10000)
		{
			return -1;
		}
	}	
	return j;
}

unsigned long long cec_ucs2be_to_utf8(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size)
{
	unsigned short present_char = 0;
	unsigned long long i = 0, j = 0;
	for (;;)
	{
		if (i >= size)
		{
			break;
		}
		present_char = *(unsigned short*)(load_buffer + i);   //The target opcodes should be ... 	
														  /*
                                amd64 ->mov rbx, qword ptr ds:[&i]
                                        mov ax, word ptr ds:[load_buffer + rbx]
                                        mov word ptr ds:[present_char], ax
                                        ...

                                        *"rax/rbx" is a example register for 64bits x86 assembly language(amd64).
														  */

		i += 2;
		

		unsigned char tmp2;
		tmp2 = *((unsigned char*)(&present_char) + 1) ;
		present_char <<= 8;
		present_char += tmp2;

		unsigned char* present_char_upper8 = (unsigned char*)&present_char + 1;

		if (present_char < 0x80)
		{
			store_buffer[j] = *(unsigned char*)(&present_char);
			j++;
			continue;
		}
		if (present_char >= 0x80 && present_char < 0x800)
		{
			store_buffer[j] = 0xc0;
			unsigned char tmp[2];
			tmp[0] = *present_char_upper8;
			tmp[0] <<= 2;
			tmp[1] = *(unsigned char*)(&present_char);
			tmp[1] >>= 6;
			tmp[1] &= 0x03;   //for the safety
			store_buffer[j] += tmp[1];
			j++;

			store_buffer[j] = 0x80;
			tmp[1] = *(unsigned char*)(&present_char);
			tmp[1] &= 0x3f;
			store_buffer[j] += tmp[1];
			j++;
			continue;
		}
		if (present_char >= 0x800 && present_char < 0x10000)
		{
			store_buffer[j] = 0xe0;
			unsigned char tmp[3];
			tmp[0] = *present_char_upper8;
			tmp[0] >>= 4;
			tmp[0] &= 0x0f;   //for the safety
			store_buffer[j] += tmp[0];
			j++;

			store_buffer[j] = 0x80;
			tmp[1] = *present_char_upper8;
			tmp[1] &= 0x0f;
			tmp[1] <<= 2;
			store_buffer[j] += tmp[1];
			tmp[2] = *(unsigned char*)(&present_char);
			tmp[2] >>= 6;
			tmp[2] &= 0x03;   //for the safety
			store_buffer[j] += tmp[2];
			j++;

			store_buffer[j] = 0x80;
			tmp[2] = *(unsigned char*)(&present_char);
			tmp[2] &= 0x3f;
			store_buffer[j] += tmp[2];
			j++;

			continue;
		}
		if (present_char >= 0x10000)
		{
			return -1;
		}
	}
	return j;
}
