#pragma once
//
//    FILE: horter.h
// PURPOSE: Arduino library for all Horter Raspberry SPS Port-Expansions


#include "Arduino.h"
#include "Wire.h"

// I2HE - 8 channel I2C Input expander
#ifndef HORTER_I2HE_INITIAL_VALUE
#define HORTER_I2HE_INITIAL_VALUE       0xFF
#endif

#define HORTER_I2HE_OK                  0x00
#define HORTER_I2HE_PIN_ERROR           0x81
#define HORTER_I2HE_I2C_ERROR           0x82

// I2HA - 8 channel I2C Output expander
#ifndef HORTER_I2HA_INITIAL_VALUE
#define HORTER_I2HA_INITIAL_VALUE       0xFF
#endif

#define HORTER_I2HA_OK                  0x00
#define HORTER_I2HA_PIN_ERROR           0x81
#define HORTER_I2HA_I2C_ERROR           0x82

// I2HAE - 8 channel I2C analog Input expander
#ifndef HORTER_I2HAE_INITIAL_VALUE
#define HORTER_I2HAE_INITIAL_VALUE       { 0x0, 0x0, 0x0, 0x0, 0x0 }
#endif

#define HORTER_I2HAE_OK                  0x00
#define HORTER_I2HAE_PIN_ERROR           0x81
#define HORTER_I2HAE_I2C_ERROR           0x82

// I2HAA - 4 channel I2C analog output expander
#ifndef HORTER_I2HAA_INITIAL_VALUE
#define HORTER_I2HAA_INITIAL_VALUE       { 0x0, 0x0, 0x0, 0x0 }
#endif

#define HORTER_I2HAA_OK                  0x00
#define HORTER_I2HAA_PIN_ERROR           0x81
#define HORTER_I2HAA_I2C_ERROR           0x82

namespace horter {

#pragma region I2HE   // I2HE - 8 channel I2C Input expander

  class I2HE
  {
  public:

    uint8_t data;

    explicit I2HE(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

    bool    begin(uint8_t value = HORTER_I2HE_INITIAL_VALUE);
    bool    isConnected();

    //  note: setting the address corrupt internal buffer values
    //  a read8() / write8() call updates them.
    bool    setAddress(const uint8_t deviceAddress);
    uint8_t getAddress();

    uint8_t read8();
    uint8_t read(const uint8_t pin);
    uint8_t value() const { return data; };

    int     lastError();

  private:

    uint8_t _address;
    int     _error;

    TwoWire*  _wire;
    
  };

#pragma endregion I2HE

#pragma region I2HA   // I2HA - 8 channel I2C Output expander

  class I2HA
  {
  public:

    uint8_t data;
    
    explicit I2HA(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

    bool    begin(uint8_t value = HORTER_I2HA_INITIAL_VALUE);
    bool    isConnected();

    //  note: setting the address corrupt internal buffer values
    //  a read8() / write8() call updates them.
    bool    setAddress(const uint8_t deviceAddress);
    uint8_t getAddress();

    void    write8(const uint8_t value);
    void    write(const uint8_t pin, const uint8_t value);

    //  rotate, shift, toggle, reverse expect all lines are output
    void    toggle(const uint8_t pin);
    //      default 0xFF ==> invertAll()
    void    toggleMask(const uint8_t mask = 0xFF);
    void    shiftRight(const uint8_t n = 1);
    void    shiftLeft(const uint8_t n = 1);
    void    rotateRight(const uint8_t n = 1);
    void    rotateLeft(const uint8_t n = 1);
    void    reverse();

    void    select(const uint8_t pin);
    void    selectN(const uint8_t pin);
    void    selectNone() { write8(0x00); };
    void    selectAll()  { write8(0xFF); };

    int     lastError();

  private:

    uint8_t _address;
    int     _error;

    TwoWire*  _wire;
    
  };

#pragma endregion I2HA

#pragma region I2HAE  // I2HAE - 8 channel I2C analog Input expander

  class I2HAE
  {
  public:

    uint16_t data[5] = HORTER_I2HAE_INITIAL_VALUE;

    explicit I2HAE(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

    bool    begin();
    bool    isConnected();

    //  note: setting the address corrupt internal buffer values
    //  a read8() / write8() call updates them.
    bool    setAddress(const uint8_t deviceAddress);
    uint8_t getAddress();

    uint16_t* read5();
    uint16_t read(const uint8_t channel);
    uint16_t* value() { return data; };

    int     lastError();

  private:

    uint8_t _address;
    int     _error;

    TwoWire*  _wire;
    
  };

#pragma endregion I2HAE

#pragma region I2HAA  // I2HAA - 4 channel I2C analog output expander

  class I2HAA
  {
  public:

    uint16_t  data[4] = HORTER_I2HAA_INITIAL_VALUE;

    explicit  I2HAA(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

    bool      begin();
    bool      isConnected();

    //  note: setting the address corrupt internal buffer values
    //  a read8() / write8() call updates them.
    bool      setAddress(const uint8_t deviceAddress);
    uint8_t   getAddress();

    void      write4(const uint16_t value[4]);
    void      write(const uint8_t channel, const uint16_t value);
    uint16_t* valueOut() { return data; }

    int       lastError();

  private:

    uint8_t   _address;
    int       _error;

    TwoWire*  _wire;
    
  };

#pragma endregion I2HAA

}

//  -- END OF FILE --

