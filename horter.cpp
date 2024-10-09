//
//    FILE: horter.cpp
// PURPOSE: Arduino library for all Horter Raspberry SPS Port-Expansions


#include "horter.h"

namespace horter {

#pragma region I2HE   // I2HE - 8 channel I2C Input expander

  I2HE::I2HE(const uint8_t deviceAddress, TwoWire *wire)
  {
    _address    = deviceAddress;
    _wire       = wire;
    data     = 0;
    _error      = HORTER_I2HE_OK;
  }

  bool I2HE::begin(uint8_t value)
  {
    if (! isConnected()) return false;
    return true;
  }

  bool I2HE::isConnected()
  {
    _wire->beginTransmission(_address);
    return ( _wire->endTransmission() == 0);
  }

  bool I2HE::setAddress(const uint8_t deviceAddress)
  {
    _address = deviceAddress;
    return isConnected();
  }

  uint8_t I2HE::getAddress()
  {
    return _address;
  }

  //  removed _wire->beginTransmission(_address);
  //  with    @100 KHz -> 265 micros()
  //  without @100 KHz -> 132 micros()
  //  without @400 KHz -> 52 micros()
  //  TODO    @800 KHz -> ??
  uint8_t I2HE::read8()
  {
    if (_wire->requestFrom(_address, 1U) != 1)
    {
      _error = HORTER_I2HE_I2C_ERROR;
      return data;  //  last value
    }
    // get Complement
    data = -(_wire->read() + 1);
    return data;
  }

  uint8_t I2HE::read(const uint8_t pin)
  {
    if (pin > 7)
    {
      _error = HORTER_I2HE_PIN_ERROR;
      return 0;
    }
    I2HE::read8();
    return (data & (1 << pin)) > 0;
  }

  int I2HE::lastError()
  {
    int e = _error;
    _error = HORTER_I2HE_OK;  //  reset error after read, is this wise?
    return e;
  }

#pragma endregion I2HE

#pragma region I2HA   // I2HA - 8 channel I2C Output expander

  I2HA::I2HA(const uint8_t deviceAddress, TwoWire *wire)
  {
    _address    = deviceAddress;
    _wire       = wire;
    data    = 0xFF;
    _error      = HORTER_I2HA_OK;
  }

  bool I2HA::begin(uint8_t value)
  {
    if (! isConnected()) return false;
    I2HA::write8(value);
    return true;
  }

  bool I2HA::isConnected()
  {
    _wire->beginTransmission(_address);
    return ( _wire->endTransmission() == 0);
  }

  bool I2HA::setAddress(const uint8_t deviceAddress)
  {
    _address = deviceAddress;
    return isConnected();
  }

  uint8_t I2HA::getAddress()
  {
    return _address;
  }

  void I2HA::write8(const uint8_t value)
  {
    data = value;
    _wire->beginTransmission(_address);
    _wire->write(-(data + 1));
    _error = _wire->endTransmission();
  }

  void I2HA::write(const uint8_t pin, const uint8_t value)
  {
    if (pin > 7)
    {
      _error = HORTER_I2HA_PIN_ERROR;
      return;
    }
    if (value == LOW)
    {
      data &= ~(1 << pin);
    }
    else
    {
      data |= (1 << pin);
    }
    write8(data);
  }

  void I2HA::toggle(const uint8_t pin)
  {
    if (pin > 7)
    {
      _error = HORTER_I2HA_PIN_ERROR;
      return;
    }
    toggleMask(1 << pin);
  }

  void I2HA::toggleMask(const uint8_t mask)
  {
    data ^= mask;
    I2HA::write8(data);
  }

  void I2HA::shiftRight(const uint8_t n)
  {
    if ((n == 0) || (data == 0)) return;
    if (n > 7)         data = 0;     //  shift 8++ clears all, valid...
    if (data != 0) data >>= n;   //  only shift if there are bits set
    I2HA::write8(data);
  }

  void I2HA::shiftLeft(const uint8_t n)
  {
    if ((n == 0) || (data == 0)) return;
    if (n > 7)         data = 0;    //  shift 8++ clears all, valid...
    if (data != 0) data <<= n;  //  only shift if there are bits set
    I2HA::write8(data);
  }

  int I2HA::lastError()
  {
    int e = _error;
    _error = HORTER_I2HA_OK;  //  reset error after read, is this wise?
    return e;
  }

  void I2HA::rotateRight(const uint8_t n)
  {
    uint8_t r = n & 7;
    if (r == 0) return;
    data = (data >> r) | (data << (8 - r));
    I2HA::write8(data);
  }

  void I2HA::rotateLeft(const uint8_t n)
  {
    rotateRight(8 - (n & 7));
  }

  void I2HA::reverse()  //  quite fast: 4 and, 14 shifts, 3 or, 3 assignment.
  {
    uint8_t x = data;
    x = (((x & 0xAA) >> 1) | ((x & 0x55) << 1));
    x = (((x & 0xCC) >> 2) | ((x & 0x33) << 2));
    x =          ((x >> 4) | (x << 4));
    I2HA::write8(x);
  }

  void I2HA::select(const uint8_t pin)
  {
    uint8_t n = 0x00;
    if (pin < 8) n = 1 << pin;
    write8(n);
  };

  void I2HA::selectN(const uint8_t pin)
  {
    uint8_t n = 0xFF;
    if (pin < 8) n = (2 << pin) - 1;
    write8(n);
  };


#pragma endregion I2HA

#pragma region I2HAE  // I2HAE - 5 channel I2C analog Input expander

  I2HAE::I2HAE(const uint8_t deviceAddress, TwoWire *wire)
  {
    _address    = deviceAddress;
    _wire       = wire;
    _error      = HORTER_I2HAE_OK;
  }

  bool I2HAE::begin()
  {
    if (! isConnected()) return false;
    return true;
  }

  bool I2HAE::isConnected()
  {
    _wire->beginTransmission(_address);
    return ( _wire->endTransmission() == 0);
  }

  bool I2HAE::setAddress(const uint8_t deviceAddress)
  {
    _address = deviceAddress;
    return isConnected();
  }

  uint8_t I2HAE::getAddress()
  {
    return _address;
  }

  //  removed _wire->beginTransmission(_address);
  //  with    @100 KHz -> 265 micros()
  //  without @100 KHz -> 132 micros()
  //  without @400 KHz -> 52 micros()
  //  TODO    @800 KHz -> ??
  uint16_t* I2HAE::read5()
  {
    uint8_t raw[11];
    if (_wire->requestFrom(_address, 11U) != 11)
    {
      _error = HORTER_I2HAE_I2C_ERROR;
      return data;  //  last value
    }

    if (_wire->available()) {
      _wire->readBytes(raw, 11);

      for (int i = 0; i < 5; i++)
      {
        data[i] = raw[(i * 2) + 1] | (raw[(i * 2) + 2] << 8);
      }
    }

    return data;
  }

  uint16_t I2HAE::read(const uint8_t channel)
  {
    I2HAE::read5();
    return data[channel];
  }

  int I2HAE::lastError()
  {
    int e = _error;
    _error = HORTER_I2HAE_OK;  //  reset error after read, is this wise?
    return e;
  }

#pragma endregion I2HAE

#pragma region I2HAA  // I2HAA - 4 channel I2C analog output expander

  I2HAA::I2HAA(const uint8_t deviceAddress, TwoWire *wire)
  {
    _address    = deviceAddress;
    _wire       = wire;
    _error      = HORTER_I2HAA_OK;
  }

  bool I2HAA::begin()
  {
    if (! isConnected()) return false;
    I2HAA::write4(data);
    return true;
  }

  bool I2HAA::isConnected()
  {
    _wire->beginTransmission(_address);
    return ( _wire->endTransmission() == 0);
  }

  bool I2HAA::setAddress(const uint8_t deviceAddress)
  {
    _address = deviceAddress;
    return isConnected();
  }

  uint8_t I2HAA::getAddress()
  {
    return _address;
  }

  void I2HAA::write4(const uint16_t value[4])
  {
    for (size_t i = 0; i < 4; i++)
    {
      data[i] = value[i];
    }
    
    uint8_t raw[9];
    raw[0] = 0;
    raw[1] = data[0];
    raw[2] = data[0] >> 8;
    raw[3] = data[1];
    raw[4] = data[1] >> 8;
    raw[5] = data[2];
    raw[6] = data[2] >> 8;
    raw[7] = data[3];
    raw[8] = data[3] >> 8;

    _wire->beginTransmission(_address);
    _wire->write(raw, 9);
    _error = _wire->endTransmission();
  }

  void I2HAA::write(const uint8_t channel, const uint16_t value)
  {
    data[channel] = value;
    write4(data);
  }

  int I2HAA::lastError()
  {
    int e = _error;
    _error = HORTER_I2HAA_OK;  //  reset error after read, is this wise?
    return e;
  }

#pragma endregion I2HAA


}

//  -- END OF FILE --

