/**************************************************************************************************/

/* i2c_helper::read */

_i2c_dev::init();
_i2c_dev::sendStart();

for (int j = 0; j < numbByte; j++) {
	if (_i2c_dev::send((unsigned char)devAddr)) {
		if (_i2c_dev::send((unsigned char)(regAddr + j))) {
			_i2c_dev::sendRepeatedStart();
			unsigned char sl_addrr = (unsigned char)(devAddr + 1);
			if (_i2c_dev::send((unsigned char)sl_addrr)) {
				*(buf + j) = _i2c_dev::recvWithNack();
				_i2c_dev::sendStop();
				delayUs(10);
			}
		}
		else
			return false;
	}
	else
		return false;
}
return true;
}



/* i2c_helper::write */

_i2c_dev::init();
_i2c_dev::sendStart();

for (int j = 0; j < numByte; j++) {
	if (_i2c_dev::send((unsigned char)devAddr)) {
		if (_i2c_dev::send((unsigned char)(regAddr + j))) {
			if(!_i2c_dev::send((unsigned char) * (buf + j)))
				return false;
		}
		else
			return false;
	}
	else
		return false;

}
_i2c_dev::sendStop();
delayUs(10);
return true;
}


/**************************************************************************************************/

/* LSM303AGRAccMag::io_read() */

bool io_read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead, 
				uint8_t dev_address , uint8_t reg_mask = 0xFF)
{
	uint8_t i = 0;
	for (i = 0; i < NumByteToRead; i++) {
		if (!((I2CHelper::getInstance())->read(pBuffer + i,
											  dev_address, 
											  RegisterAddr + i, 1)))
			return false;
		*(pBuffer + i) &= reg_mask;
	}
	return true;
}


/* LSM303AGRAccMag::io_write() */

bool io_write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite,
				uint8_t dev_address , uint8_t reg_mask = 0xFF)
{
	uint8_t i = 0;
	uint8_t * tmp_buf = new uint8_t[NumByteToWrite];
	for (i = 0; i < NumByteToWrite; i++) {
		if (!((I2CHelper::getInstance())->read((tmp_buf + i),
							dev_address, 
							RegisterAddr + i, 1)))
			return false;
		else {
			*(tmp_buf + i) &= ~reg_mask;
			*(tmp_buf + i) |= *(pBuffer + i);

			if (!((I2CHelper::getInstance())->write((tmp_buf + i), 
							 dev_address, 
							 RegisterAddr + i,
							 1, false)))
				return false;
		}
	}
	delete[] tmp_buf;
	return true;
}
