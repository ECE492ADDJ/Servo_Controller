--Filename:       servo.vhd
--File type:      client-side vhdl code
--Author:         David Ross
--Description:    Hardware description for the servo control 
library ieee;
-- Commonly imported packages:

-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.std_logic_1164.all;

-- SIGNED and UNSIGNED types, and relevant functions
use ieee.numeric_std.all;

-- Basic sequential functions and concurrent procedures
use ieee.VITAL_Primitives.all;

-- Allow + with vectors
use IEEE.std_logic_signed.all;

use work.DE2_CONSTANTS.all;

entity servo is

	port
	(
		-- clock interface
		clk			:	in std_logic;
		
		-- reset interface
		reset_n		:	in std_logic;
		
		-- gpio interface, need 1 pin for the pwm control
		coe_gpio0	:	out DE2_GPIO_0_BUS;
	
		-- avalon slave interface
		-- write current angle to memory
		avs_s0_read_n	: in std_logic ;
		avs_s0_readdata : out std_logic_vector (31 downto 0);
		
		-- read new angle from memory
		avs_s0_write_n	: in std_logic ;
		avs_s0_writedata : in std_logic_vector (31 downto 0)
	);
end servo;

architecture avalon of servo is

-- Real value is 8.33. Might need to change it later
constant msPerDegree : integer := 8;
-- Frequency is scaled down to get correct timing with all other values scaled up to prevent use of floats
constant frequency: integer := 50;

-- Current rotation of the motor
signal currentPulseWidth : integer := 2000;
signal numDegrees : integer := 0;
-- pulse time = pulseWidth/clockperiod
-- Upper bound is 2.5ms / 20ns = 125 000
-- Lower bound is 0.5ms / 20ns = 25 000
-- Middle is 2ms/20ns = 75 000

begin
-- If memory has a new value or a key was pressed, change the pulse width
change_pulse: process(avs_s0_read_n) is
	begin	
		if avs_s0_write_n = '0' then
			numDegrees <= to_integer(signed(avs_s0_writedata));
		end if;
end process;

-- When a new angle is read, calculate new width and enforce bounds
calculatePulse: process(numDegrees) is
	begin
		currentPulseWidth <= 1500 + numDegrees * msPerDegree;
end process;

send_pulse: process(clk) is
	variable pulseTime : integer := 75000;
	variable refreshTime : integer := 0;

	begin
	if rising_edge(clk) then
		refreshTime := refreshTime + 1;
		
		if (refreshTime >= 250000) then
			refreshTime := 0;
			pulseTime := currentPulseWidth * frequency;
		end if;
		
		if(pulseTime > 0) then
				coe_gpio0(0) <= '1';
				pulseTime := pulseTime - 1;
		else
				coe_gpio0(0) <= '0';
		end if;
		
	end if;
	
	

	
	
end process;


end avalon;


library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
	use ieee.std_logic_1164.all;

package DE2_CONSTANTS is
	
	type DE2_SDRAM_ADDR_BUS is array(11 downto 0) of std_logic;
	type DE2_SDRAM_DATA_BUS is array(15 downto 0) of std_logic;
	
	type DE2_LCD_DATA_BUS	is array(7 downto 0) of std_logic;
	
	type DE2_LED_GREEN		is array(7 downto 0)  of std_logic;
	type DE2_GPIO_0_BUS 		is array(0 downto 0) of std_logic;
	
	type DE2_SRAM_ADDR_BUS	is array(17 downto 0) of std_logic;
	type DE2_SRAM_DATA_BUS  is array(15 downto 0) of std_logic;
	
end DE2_CONSTANTS;




