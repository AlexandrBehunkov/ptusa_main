LIS = 0 --����� ������ � �������� ���������.
LNO = 1 --����� ������ � ���������� ���������.

--����� ��������������� ������ �� ���������� ���������� �� ���������.
project_tech_object =
    {
    name         = "TANK",
    number       = 1,
    states_count = 32,

    timers_count               = 1,
    params_float_count         = 1,
    runtime_params_float_count = 1,
    params_uint_count          = 1,
    runtime_params_uint_count  = 1,

    sys_tech_object = 0,
    }

--�������� ���������� ������, ��� ���� ������� ��������������� ���������
--��������������� ������ �� �++.
function project_tech_object:new( o )

    o = o or {} -- create table if user does not provide one
    setmetatable( o, self )
    self.__index = self

    print( "project_tech_object:new( o ), n = ", o.number )

   --������� ��������� ������.
    o.sys_tech_object = tech_object( o.name, o.number, o.states_count,
        o.timers_count, o.params_float_count, o.runtime_params_float_count,
        o.params_uint_count, o.runtime_params_uint_count )
    return o
end

--�������� ��� �������, ��� ������ �� ������, ���������� ���� �� �����������
--����� � ������� (���� main.lua).
function project_tech_object:exec_cmd( cmd )
    return 0
end

function project_tech_object:check_on_mode( mode )
    return 0
end

function project_tech_object:init_mode( mode )
    return 0
end

function project_tech_object:evaluate( par )
   return 0
end

function project_tech_object:check_off_mode( mode )
    return 0
end

function project_tech_object:final_mode( mode )
    return 0
end

function project_tech_object:init_params( par )
    return 0
end

function project_tech_object:init_runtime_params( par )
    return 0
end

--�������, ������� �������������� � ������ ��������������� �������
--���������� ���������������� ������� (���������� �� �++).

function project_tech_object:get_number()
    return self.sys_tech_object:get_number()
end

function project_tech_object:get_modes_count()
    return self.sys_tech_object:get_modes_count()
end

function project_tech_object:get_mode( mode )
    return self.sys_tech_object:get_mode( mode )
end

function project_tech_object:set_mode( mode, new_state )
    return self.sys_tech_object:set_mode( mode, new_state )
end

--������������� ���� ��������� ���������������� ��������������� ��������.
object_manager =
    {
    objects = {}, --���������������� ��������������� �������.

    --���������� ����������������� ���������������� �������.
    add_object = function ( self, new_object )
        self.objects[ #self.objects + 1 ] = new_object
    end,

    --��������� ���������� ���������������� ��������������� ��������.
    get_objects_count = function( self )
        return #self.objects
    end,

    --��������� ����������������� ���������������� �������.
    get_object = function( self, object_idx )
        local res = self.objects[ object_idx ]
        if res then
            return self.objects[ object_idx ].sys_tech_object
        else
            return 0
        end
    end
    }
-- ----------------------------------------------------------------------------
-- ----------------------------------------------------------------------------
-- ������ "������ ���������".
-- ----------------------------------------------------------------------------

--testing = 1 --��������������� ��� ������������ � ���������� Lua.
if testing == 1 then require ( test ) end

-- ----------------------------------------------------------------------------
-- ����� ��������.
-- ----------------------------------------------------------------------------
--������� ��������.
CONSTANTS =
    {
    COMB_IDX  = 0,

    TANK1_IDX = 1,
    TANK2_IDX = 2,
    TANK3_IDX = 3,
    TANK4_IDX = 4,

    POST1_IDX = 5,
    POST2_IDX = 6,
    }

--�������.
CMD =
    {
    RESET_POST                 = 30, -- ��������� �����/������, �������� ������ ���������.
    SET_PAUSE_AND_HEATING      = 31, -- �������� ����� � ��������.
    RESET_TANK_POST1           = 32, -- ��������� ����� ������ ��� ����� �, ���� ����, ����� 1.
    RESET_TANK_POST2           = 33, -- ��������� ����� ������ ��� ����� �, ���� ����, ����� 2.

    SET_POST1_AND_TANK         = 34, -- �������� ���� � ���� 1.
    SET_POST2_AND_TANK         = 35, -- �������� ���� � ���� 2.
    SET_HEATING_POST1_AND_TANK = 36, -- �������� ���� � �������� ���� 1.
    SET_HEATING_POST2_AND_TANK = 37, -- �������� ���� � �������� ���� 2.
    }
-- ----------------------------------------------------------------------------
-- �������� - ���� ���������.
-- ----------------------------------------------------------------------------
whey_tank = project_tech_object:new()

whey_tank.states_count               = 20
whey_tank.timers_count               = 5
whey_tank.runtime_params_float_count = 10

--���������.
whey_tank.PARAMETERS =
    {
    WARNING_REASON = 4
    }

--������.
whey_tank.WARNINGS =
    {
    TANK_IN_STOP  = 1,
    TANK_IN_START = 2
    }

--������.
whey_tank.MODES =
    {
    WHEY_ACCEPTING  = 0, --������ ���������.
    WHEY_OUT_P1     = 1, --������ ��������� ���� 1.
    WHEY_OUT_P2     = 2, --������ ��������� ���� 2.
    WASH            = 3, --�����.

    WHEY_WACCEPTING = 8, --�������� ������ ���������.
    WHEY_WOUT_P1    = 9, --�������� ������ ��������� ���� 1.
    WHEY_WOUT_P2    = 10,--�������� ������ ��������� ���� 2.
    }
-- ----------------------------------------------------------------------------
-- �������� - ����.
-- ----------------------------------------------------------------------------
post = project_tech_object:new()

post.states_count               = 20
post.timers_count               = 5
post.params_float_count         = 5
post.runtime_params_float_count = 20

--���������.
post.RT_F_PARAMETERS =
    {
    SECTION1      = 1, -- ������� ������ 1.
    SECTION2      = 2,
    SECTION3      = 3,
    SECTION4      = 4,
    SECTION5      = 5,
    SECTION6      = 6,
    SECTION7      = 7,
    SECTION8      = 8,
    SECTION9      = 9,
    SECTION10     = 10,
    AUTO_NUM      = 11, -- ����� ������.
    CURRENT_SEC   = 12, -- ������� ������.
    SECTIONS      = 13, -- ����� ����� ������.
    TOT_VOL       = 14, -- ����� �����.
    IS_RESET_POST = 15, -- ��� ��������� ����� �������� �������.

    WARNING_REASON = 17
    }
post.S_F_PARAMETERS =
    {
    -- ����������� ������� � �����, ���� �������� ��������� �
    -- ���������� ����� �������.
    LE_MIN         = 1,

    -- ������������ ����������� ������, ��� ���������� �������
    -- ��������� ��������� ��������.
    T_OUT_MAX      = 2,

    -- ����������� ������, ��� ���������� ������� ����������� �������
    -- ������ ���������.
    OUT_NORMAL     = 3,

    PAUSE_TIME_MAX = 4 -- ������������ ����� ������ �����, ���.
    }

--������.
post.WARNINGS =
    {
    MAX_OUT_TEMPER  = 1,
    NO_FLOW         = 2,
    PAUSE_TIME_LEFT = 3
    }

--������.
post.MODES =
    {
    WHEY_HEATING         = 16, -- �������� ��������� ������.
    WHEY_ACCEPTING       = 17, -- ������ ��������� ������.
    WHEY_ACCEPTING_PAUSE = 18, -- ����� ������ ��������� ������.
    WHEY_ACCEPTING_END   = 19, -- ���������� ������ ��������� ������.
    }

--�������.
post.TIMERS =
    {
    DELAY_TEMP = 1,
    PAUSE_TIME = 2
    }

-- ----------------------------------------------------------------------------
-- �������� - ��������.
-- ----------------------------------------------------------------------------
comb = project_tech_object:new()

-- ----------------------------------------------------------------------------
-- ������ ����� ���������.
-- ----------------------------------------------------------------------------
function whey_tank:init()
    --�������.
    self.V1 = V( self.number * 100 + 1 )
    self.V2 = V( self.number * 100 + 2 )
    self.V3 = V( self.number * 100 + 3 )
    self.V4 = V( self.number * 100 + 4 )
    self.V5 = V( self.number * 100 + 5 )
    self.V7_1 = ( 1 == self.number or 2 == self.number ) and V( 107 )  or V( 507 )
    self.V7_2 = ( 1 == self.number or 2 == self.number ) and V( 207 )  or V( 307 )
    self.LSL = LS( self.number * 100 + 1 )
    self.LE1 = LE( self.number )
    self.LSH = LS( self.number * 100 + 2 )
    self.T = TE( self.number )

    self.V_1 = nil
    self.V_2 = nil
    self.V_3 = nil
    self.V_4 = nil
end
-- ----------------------------------------------------------------------------
function whey_tank:final_mode( mode )
    if mode == self.MODES.WHEY_ACCEPTING then
        self.V1:off()
        self.V2:off()
    end

    if mode == self.MODES.WHEY_OUT_P1 then
        self.V1:off()

        if 1 == number or 2 == number then
            self.V4:off()
        else
            self.V3:off()
            V( 207 ):off()
            V( 307 ):off()
            self.V_3:off()
            self.V_4:off()
        end
    end

    if mode == self.MODES.WHEY_OUT_P2 then
        self.V1:off()

        if 3 == number or 4 == number then
            self.V4:off()
        else
            self.V3:off()

            V( 207 ):off()
            V( 307 ):off()

            self.V_3:off()
            self.V_4:off()
        end
    end

    if mode == self.MODES.WASH then
        self.V1:off()
        self.V3:off()
        self.V5:off()

        self.V7_2:off()
        V( 206 ):off()
        V( 208 ):off()
        V( 109 ):off()
        N( 2202 ):off()
    end

    --��� ���������� ������/������ �������� ��� ������� ���������
    --������/������.
    if mode >= self.MODES.WHEY_ACCEPTING and mode <= self.MODES.WHEY_OUT_P2 then
        idx = G_TECH_OBJECT_MNGR():get_object_with_active_mode(
            mode + self.MODES.WHEY_WACCEPTING, CONSTANTS.TANK1_IDX, CONSTANTS.TANK2_IDX )

        if idx >= 0 then
            G_TECH_OBJECTS( idx ):set_mode( mode + self.MODES.WHEY_WACCEPTING, 0 )
            G_TECH_OBJECTS( idx ):set_mode( mode, 1 )
            if self.MODES.WHEY_ACCEPTING == mode then
                G_TECH_OBJECTS( idx ).rt_par_float:save(
                    self.PARAMETERS.WARNING_REASON, self.WARNINGS.TANK_IN_START )
            end
        else
            if self.MODES.WHEY_ACCEPTING == mode then
                self.sys_tech_object.rt_par_float:save( self.PARAMETERS.WARNING_REASON,
                    self.WARNINGS.TANK_IN_STOP )
            end
        end
    end
    --��� ���������� ������/������ �������� ��� ������� ���������
    --������/������-!>

    return 0
end
-- ----------------------------------------------------------------------------
function whey_tank:on_whey_out( out_post )
    self.V1:on()

    if ( out_post == POST1 and ( 1 == self.number or 2 == self.number ) ) or
        ( out_post == POST2 and ( 3 == self.number or 4 == self.number ) ) then
        self.V3:off()
        if out_post:get_mode( post.MODES.WHEY_ACCEPTING ) == 1 then
            self.V4:on()
        else
            self.V4:off()
        end
    else
        self.V3:on()
        self.V4:off()

        V( 107 ):off()
        V( 207 ):on()
        V( 206 ):off()
        V( 307 ):on()
        V( 507 ):off()

        self.V_1:off()
        self.V_2:off()
        self.V_3:on()
        if out_post:get_mode( post.MODES.WHEY_ACCEPTING ) == 1 then
            self.V_4:on()
        else
            self.V_4:off()
        end
    end

    if self.LSL:get_state() == LNO or
        ( out_post:get_mode( post.MODES.WHEY_ACCEPTING ) == 1 and
        out_post.flow:get_state() == 0 ) then
        local idx = G_TECH_OBJECT_MNGR():get_object_with_actiVe_mode(
            self.MODES.WHEY_WOUT_P1, CONSTANTS.TANK1_IDX, CONSTANTS.TANK2_IDX )

        if idx >= 0 and
            self.LE1:get_value() <
            out_post.par_float:get_value( post.PARAMETERS.S_F__LE_MIN ) then
            self:set_mode( self.MODES.WHEY_OUT_P1, 0 )
        else
            out_post:set_mode( post.MODES.WHEY_ACCEPTING_PAUSE, 1 )
            out_post.sys_tech_object.rt_par_float:save( post.PARAMETERS.RT_F__WARNING_REASON,
                post.WARNINGS.NO_FLOW )
        end
    end
end
-- ----------------------------------------------------------------------------
function whey_tank:evaluate( par )
    for i = 0, self:get_modes_count() do

        if self:get_mode( i ) == 1 then

            if i == self.MODES.WHEY_ACCEPTING then
                self.V1:on()
                self.V2:on()
                self.V4:off()
                V( 106 ):off()

                if self.LSH:get_state() == LIS then
                    self:set_mode( self.MODES.WHEY_ACCEPTING, 0 )
                end
            end

            if i == self.MODES.WHEY_OUT_P1 then
                self:on_whey_out( POST1 )
            end


            if i == self.MODES.WHEY_OUT_P2 then
                self:on_whey_out( POST2 )
            end

            if i == self.MODES.WASH then
                self.V1:on()
                self.V2:off()
                self.V3:on()
                self.V4:off()
                self.V5:on()
                self.V7_1:off()
                self.V7_2:on()
                V( 108 ):off()
                V( 206 ):on()
                V( 208 ):on()
                V( 109 ):on()
                N( 2202 ):on()
            end

            if i == self.MODES.WHEY_WACCEPTING then
            end

            if i == self.MODES.WHEY_WOUT_P1 then
            end

            if i == self.MODES.WHEY_WOUT_P2 then
            end
        end
    end

    return 0
end
-- ----------------------------------------------------------------------------
function whey_tank:exec_cmd( cmd )

    local idx = -1

    if cmd == CMD.RESET_TANK_POST1 then
        idx = G_TECH_OBJECT_MNGR():get_object_with_active_mode(
            self.MODES.WHEY_WOUT_P1, CONSTANTS.TANK1_IDX, CONSTANTS.TANK2_IDX )

        if idx == -1 then
            POST1:exec_cmd( CMD.RESET_POST ) -- ���������� ���� 1.
        end
        self:set_mode( self.MODES.WHEY_OUT_P1, 0 )
    end

    if cmd == CMD.RESET_TANK_POST2 then
        idx = G_TECH_OBJECT_MNGR():get_object_with_active_mode(
            self.MODES.WHEY_WOUT_P2, CONSTANTS.TANK1_IDX, CONSTANTS.TANK2_IDX )

        if idx == -1 then
            POST2:exec_cmd( CMD.RESET_POST ) -- ���������� ���� 2.
        end
        self:set_mode( self.MODES.WHEY_OUT_P2, 0 )
    end

    if cmd == CMD.SET_POST1_AND_TANK then
        print( "exec_cmd( CMD.SET_POST1_AND_TANK )")

        if self:get_mode( self.MODES.WASH ) == 1 or   -- �� ����� ����� ����� ������.
            self:get_mode( self.MODES.WHEY_OUT_P2 ) == 1 or
            self:get_mode( self.MODES.WHEY_WOUT_P2 ) == 1 then
            return 1
        end

        if self:set_mode( self.MODES.WHEY_OUT_P1, 1 ) ==
            1000 + self.MODES.WHEY_OUT_P1 then
            POST1:set_mode( POST1.MODES.WHEY_ACCEPTING_PAUSE, 1 )
        end
    end

    if cmd == CMD.SET_POST2_AND_TANK then
        if self:get_mode ( self.MODES.WASH ) == 1 or   -- �� ����� ����� ����� ������.
            self:get_mode( self.MODES.WHEY_OUT_P1 ) == 1 or
            self:get_mode( self.MODES.WHEY_WOUT_P1 ) == 1 then
            return 1
        end

        if self:set_mode( self.MODES.WHEY_OUT_P2, 1 ) ==
            1000 + self.MODES.WHEY_OUT_P2 then
            POST2:set_mode( POST1.MODES.WHEY_ACCEPTING_PAUSE, 1 )
        end
    end

   if cmd == CMD.SET_HEATING_POST1_AND_TANK then
        print( "self:exec_cmd( CMD.SET_POST1_AND_TANK )")
        self:exec_cmd( CMD.SET_POST1_AND_TANK )
        print( "after self:exec_cmd( CMD.SET_POST1_AND_TANK )")

        POST1:set_mode( POST1.MODES.WHEY_HEATING, 1 )
    end

    if cmd == CMD.SET_HEATING_POST2_AND_TANK then
        self:exec_cmd( CMD.SET_POST2_AND_TANK )
        POST2:set_mode( POST1.MODES.WHEY_HEATING, 1 )
    end


    self.sys_tech_object.rt_par_float:save(
        self.PARAMETERS.WARNING_REASON, 0 )

    return 1000 + cmd
end
-- ----------------------------------------------------------------------------
-- ������ �����.
-- ----------------------------------------------------------------------------
function post:init()
    --�������.
    self.V1     = V( self.number - 5 )
    self.outTE  = ( 6 == self.number or TE( 5 ) ) and TE( 6 )
    self.N1     = N( self.number - 5 )
    self.ctr    = CTR( self.number - 5 )
    self.flow   = ( 6 == self.number or FB( 5 ) ) and FB( 6 )

    self.lampReady  = ( 6 == self.number or UPR( 1 ) ) and UPR( 3 )
    self.lampWorking= ( 6 == self.number or UPR( 2 ) ) and UPR( 4 )
    self.btnStart   = ( 6 == self.number or FB( 1 ) ) and FB( 3 )
    self.btnPause   = ( 6 == self.number or FB( 2 ) ) and FB( 4 )

end
-- ----------------------------------------------------------------------------
-- �������� ����������� ��������.
-- ----------------------------------------------------------------------------
TANK1 = whey_tank:new{ number = 1 }
TANK2 = whey_tank:new{ number = 2 }
TANK3 = whey_tank:new{ number = 3 }
TANK4 = whey_tank:new{ number = 4 }
TANK1:init()
TANK2:init()
TANK3:init()
TANK4:init()

POST1 = post:new{ number = 6 }
POST2 = post:new{ number = 7 }
POST1:init()
POST2:init()

object_manager:add_object( TANK1 )
object_manager:add_object( TANK2 )
object_manager:add_object( TANK3 )
object_manager:add_object( TANK4 )
object_manager:add_object( POST1 ) -- ���� 1.
object_manager:add_object( POST2 ) -- ���� 2.

TANK6 = POST1
TANK7 = POST2
-- ----------------------------------------------------------------------------
