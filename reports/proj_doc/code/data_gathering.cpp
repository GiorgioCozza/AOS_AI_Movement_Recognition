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


/*******  LSM6DSL::get_acc_axes()  ********************************************/

bool LSM6DSLAccGyr::get_acc_axes(int32_t * aData){

    uint8_t tmp_val[6] = {0,0,0,0,0,0};
    int16_t raw_val[3] = {0,0,0};
    float sens;

    if (!LSM6DSLAccGyr::io_read((uint8_t *)tmp_val, LSM6DSL_OUTX_L_XL,6))
        return false;


    raw_val[0] = ((((int16_t)tmp_val[1]) << 8 ) + (int16_t)tmp_val[0]);
    raw_val[1] = ((((int16_t)tmp_val[3]) << 8 ) + (int16_t)tmp_val[2]);
    raw_val[2] = ((((int16_t)tmp_val[5]) << 8 ) + (int16_t)tmp_val[4]);

    if (!LSM6DSLAccGyr::get_acc_sensitivity(&sens))
        return false;

    aData[0] = (int32_t)(raw_val[0] * sens);
    aData[1] = (int32_t)(raw_val[1] * sens);
    aData[2] = (int32_t)(raw_val[2] * sens);

    return true;
}

/******************************************************************************/




bool LSM303AGRAccMag::get_acc_axes(int32_t * aData){

    uint8_t tmp_val[6] = {0,0,0,0,0,0};
    int16_t raw_val[3] = {0,0,0};
    uint8_t a_lp = 0, a_hr = 0, shift = 0;
    float sens;


    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_lp, LSM303AGR_ACC_CTRL1,
              1, LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_LP_MASK))
        return false;

    if (!LSM303AGRAccMag::io_read((uint8_t *)&a_hr, LSM303AGR_ACC_CTRL4, 1,
									LSM303AGR_ACC_I2C_ADDRESS, LSM303AGR_ACC_HR_MASK))
        return false;


/*******  LSM303AGRAccMag::get_acc_axes  ****************************************/

    if ( a_lp == LSM303AGR_ACC_LP_ENABLED && \
	     a_hr == LSM303AGR_ACC_HR_DISABLED ) {
        shift = 8;
    }else if ( a_lp == LSM303AGR_ACC_LP_DISABLED && \
               a_hr == LSM303AGR_ACC_HR_DISABLED ) {
        shift = 6;
    }else if ( a_lp == LSM303AGR_ACC_LP_DISABLED && \
   	           a_hr == LSM303AGR_ACC_HR_ENABLED ) {
        shift = 4;
    }else
        return false;

    if (!get_acc_sensitivity(&sens))
        return false;

    sens *= 1000.0f;

    if (!LSM303AGRAccMag::io_read((uint8_t *)tmp_val, LSM303AGR_ACC_OUT_X_L,6,
					LSM303AGR_ACC_I2C_ADDRESS, 0xFF))
        return false;


    raw_val[0] = (((((int16_t)tmp_val[1]) << 8 ) + (int16_t)tmp_val[0]));
    raw_val[1] = (((((int16_t)tmp_val[3]) << 8 ) + (int16_t)tmp_val[2]));
    raw_val[2] = (((((int16_t)tmp_val[5]) << 8 ) + (int16_t)tmp_val[4]));

    raw_val[0] = (((raw_val[0] >> shift) * sens + 500)) / 1000;
    raw_val[1] = (((raw_val[1] >> shift) * sens + 500)) / 1000;
    raw_val[2] = (((raw_val[2] >> shift) * sens + 500)) / 1000;


    aData[0] = (int32_t)(raw_val[0]);
    aData[1] = (int32_t)(raw_val[1]);
    aData[2] = (int32_t)(raw_val[2]);

    return true;
}

/*******************************************************************************/
