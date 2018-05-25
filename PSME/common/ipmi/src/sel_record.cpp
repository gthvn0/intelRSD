/*!
 * @brief Base class of SEL record (event)
 *
 * @header{License}
 * @copyright Copyright (c) 2017-2018 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @header{Filesystem}
 * @file ipmi/sel_record.cpp
 */

#include "ipmi/sel_record.hpp"
#include "ipmi/utils/byte_buffer.hpp"

#include <sstream>

static constexpr ipmi::command::generic::SelRecordType OEM_RECORDS_FAMILY = 0xC0;
static constexpr ipmi::command::generic::SelRecordType NON_TIMESTAMPED_OEM_RECORDS_FAMILY = 0xE0;
static constexpr unsigned RECORD_ID_POS = 0;
static constexpr unsigned RECORD_TYPE_POS = 2;
static constexpr unsigned RECORD_TIMESTAMP_POS = 3;

ipmi::command::generic::SelEntryId ipmi::SelRecord::get_id(const ipmi::SelRecord::RecordData& record) {
    return ByteBuffer::get_uint16(record, RECORD_ID_POS);
}


ipmi::command::generic::SelRecordType ipmi::SelRecord::get_type(const ipmi::SelRecord::RecordData& record) {
    return ByteBuffer::get_uint8(record, RECORD_TYPE_POS);
}


ipmi::command::generic::SelTimestamp ipmi::SelRecord::get_timestamp(const ipmi::SelRecord::RecordData& record) {
    return ByteBuffer::get_uint32(record, RECORD_TIMESTAMP_POS);
}

bool ipmi::SelRecord::has_timestamp(const ipmi::SelRecord::RecordData& record) {
    return get_type(record) < NON_TIMESTAMPED_OEM_RECORDS_FAMILY;
}


bool ipmi::SelRecord::is_oem(const RecordData& record) {
    return get_type(record) >= OEM_RECORDS_FAMILY;
}



ipmi::SelRecord::SelRecord(const ipmi::SelRecord::RecordData& record):
    id(get_id(record)), type(get_type(record)), timestamp(has_timestamp(record) ? get_timestamp(record) : 0) { }

ipmi::SelRecord::~SelRecord() { }

bool ipmi::SelRecord::has_timestamp() const {
    return get_type() < NON_TIMESTAMPED_OEM_RECORDS_FAMILY;
}


ipmi::SelRecordOem::SelRecordOem(const ipmi::SelRecord::RecordData& record): SelRecord(record), buffer(record) { }

std::string ipmi::SelRecordOem::to_string() const {
    unsigned pos;

    std::stringstream str{};
    str << "#" << std::hex << static_cast<unsigned>(get_id()) << ":: ";
    str << "OEM ";
    str << "type=" << static_cast<unsigned>(get_type());
    if (has_timestamp()) {
        pos = 7;
        str << " timestamp=" << get_timestamp();
    } else {
        pos = 3;
    }
    str << " data=";
    for (; pos < buffer.size(); pos++) {
        str << "[" << static_cast<unsigned>(buffer[pos]) << "]";
    }
    return str.str();
}



/*
 * Event record structure for Sensor (type 00-BF):
 *
 * 0-1: Record ID
 *      ID used for SEL Record access. The Record ID values 0000h and FFFFh have
 *      special meaning in the Event Access commands and must not be used as Record ID
 *      values for stored SEL Event Records
 * 2: Record type
 *      02h = system event record
 *      C0h-DFh = OEM timestamped, bytes 7-15 OEM defined
 *      E0h-FFh = OEM non-timestamped, bytes 3-15 OEM defined
 * 3-6: Time when event was logged. LS byte first.
 * 7-8: Generator ID
 *      RqSA & LUN if event was generated from IPMB. Software ID if event was generated
 *      from system software.
 *      Byte 1
 *          [7:1] - 7-bit IIC Slave Address, or 7-bit system software ID
 *          [0] 0b = ID is IPMB Slave Address
 *              1b = system software ID
 *      Byte 2
 *          [7:4] - Channel number. Channel that event message was received over. 0h if the
 *                  event message was received via the system interface, primary IPMB, or
 *                  internally generated by the BMC. (New for IPMI v1.5. These bits were reserved
 *                  in IPMI v1.0)
 *          [3:2] - reserved. Write as 00b.
 *          [1:0] - IPMB device LUN if byte 1 holds Slave Address. 00b otherwise.
 * 9: EvM Rev
 *      Event Message format version (=04h for events in this specification, 03h for IPMI
 *      v1.0 Event Messages.)
 *      Note: the BMC must accept Platform Event request messages that are in IPMI v1.0
 *      format (EvMRev=03h) and log them as IPMI v1.5 / v2.0 Records by setting the
 *      EvMRev field to 04h and setting the Channel Number in the Generator ID field
 *      appropriately for the channel that the event was received from.
 * 10: Sensor Type
 *      Sensor Type Code for sensor that generated the event
 * 11: Sensor #
 *      Number of sensor that generated the event
 * 12: EventDir and EventType
 *      Event Dir
 *          [7] 0b = Assertion event.
 *              1b = Deassertion event.
 *      Event Type
 *          [6:0] - Event Type Code
 *          Type of trigger for the event, e.g. critical threshold going high, state asserted, etc.
 *          Also indicates class of the event. E.g. discrete, threshold, or OEM. The Event Type
 *          field is encoded using the Event/Reading Type Code. See section 42.1,
 *          Event/Reading Type Codes.
 * 13-15: Event data
 *          Event Request Message Event Data Field Contents. 14/15 might not be present.
 */

static constexpr std::size_t RECORD_SIZE = 14;

static constexpr unsigned EVENT_MESSAGE_REVISION_POS = 9;
static constexpr unsigned EVENT_MESSAGE_REVISION_1_5 = 0x04;

static constexpr unsigned EVENT_SENSOR_TYPE_POS = 10;

static constexpr unsigned EVENT_SENSOR_NUMBER_POS = 11;

static constexpr unsigned EVENT_DIR_POS = 12;
static constexpr unsigned ASSERTION_EVENT_BIT = 7;

static constexpr unsigned EVENT_TYPE_POS = 12;

static constexpr unsigned EVENT_DATA_OFFSET_POS = 13;




ipmi::SelRecordGeneric::EventType ipmi::SelRecordGeneric::get_event_type(const RecordData& record) {
    return static_cast<EventType>(ByteBuffer::get_uint8(record, EVENT_TYPE_POS) & EVENT_TYPE_MASK);
}


bool ipmi::SelRecordGeneric::is_valid(const ipmi::SelRecord::RecordData& record) {
    const unsigned evm_version = ByteBuffer::get_uint8(record, EVENT_MESSAGE_REVISION_POS);
    if (evm_version != EVENT_MESSAGE_REVISION_1_5) {
        throw std::runtime_error(std::string("Invalid version ") + std::to_string(evm_version) + " of event record in SEL");
    }
    if (record.size() < RECORD_SIZE) {
        throw std::runtime_error(std::string("Offset field not in the record, size=" + std::to_string(record.size())));
    }
    return true;
}


ipmi::SelRecordGeneric::EventData ipmi::SelRecordGeneric::get_event_data(const ipmi::SelRecord::RecordData& record, unsigned pos) {
    /* if data is not available in the record, appropriate data availability should be 00b.. */
    if (record.size() < EVENT_DATA_OFFSET_POS + pos) {
        return EVENT_DATA_NOT_PRESENT;
    }

    /* data1 contains info about data availability, it is to be put in MSB in very first bits */
    unsigned shift;
    switch (pos) {
        case 2:
            shift = 8 - 6;
            break;
        case 3:
            shift = 8 - 4;
            break;
        default:
            shift = 0;
            break;
    }
    std::uint16_t data1 = ((ByteBuffer::get_uint8(record, EVENT_DATA_OFFSET_POS) << shift) & EVENT_DATA_AVAILABLE_MASK);
    if (!data1) {
        return EVENT_DATA_NOT_PRESENT;
    }
    return static_cast<EventData>(ByteBuffer::get_uint8(record, EVENT_DATA_OFFSET_POS + pos - 1) | data1);
}


ipmi::SelRecordGeneric::SelRecordGeneric(const ipmi::SelRecord::RecordData& record):
    SelRecord(record), valid_record(is_valid(record)),
    sensor_type(static_cast<ipmi::command::generic::SensorType>(ByteBuffer::get_uint8(record, EVENT_SENSOR_TYPE_POS))),
    sensor_number(ByteBuffer::get_uint8(record, EVENT_SENSOR_NUMBER_POS)),
    asserted(!ByteBuffer::is_bit_set(record, EVENT_DIR_POS, ASSERTION_EVENT_BIT)),
    event_type(get_event_type(record)),
    event_offset(static_cast<EventOffset>(ByteBuffer::get_uint8(record, EVENT_DATA_OFFSET_POS) & EVENT_DATA_OFFSET_MASK)),
    event_data2(get_event_data(record, 2)),
    event_data3(get_event_data(record, 3))
{ }


std::string ipmi::SelRecordGeneric::to_string() const {
    std::stringstream str{};
    str << "#" << std::hex << static_cast<unsigned>(get_id()) << ":: ";
    str << get_info();
    str << " sensor=" << std::hex << static_cast<unsigned>(get_sensor_number());

    str << " event_type=" << std::hex << static_cast<unsigned>(get_event_type())
        << " sensor_type=" << std::hex << static_cast<unsigned>(get_sensor_type())
        << " offset=" << std::hex << static_cast<unsigned>(get_event_offset());

    if ((get_event_data2() & EVENT_DATA_AVAILABLE_MASK) != 0) {
        str << " data=" << std::hex << static_cast<unsigned>(get_event_data2());
        if ((get_event_data3() & EVENT_DATA_AVAILABLE_MASK) != 0) {
            str << ":" << std::hex << static_cast<unsigned>(get_event_data3());
        }
    }
    if (is_asserted()) {
        str << " asserted";
    }
    else {
        str << " deasserted";
    }

    return str.str();
}


/*!
 * @brief check sensor specific data, if defined.
 *
 * There are two forms: if AVAILABLILITY is "unspecified" (in definition), only
 * plain value is checked. If AVAILABILITY bits are passed in definition, both must equal.
 *
 * @param definition
 */
bool ipmi::SelRecordGeneric::event_data_match(ipmi::SelRecordGeneric::EventData definition,
                                              ipmi::SelRecordGeneric::EventData checked) {
    if (definition != EVENT_DATA_NOT_PRESENT) {
        if ((definition & EVENT_DATA_AVAILABLE_MASK) == 0) {
            if (definition != (checked & EVENT_DATA_PLAIN_VALUE_MASK)) {
                return false;
            }
        }
        else if (definition != (checked & EVENT_DATA_VALUE_MASK)) {
            return false;
        }
    }
    return true;
}


bool ipmi::SelRecordGeneric::is_of_type(ipmi::SelRecordSensor::EventSensorType se_type) const {
    /* event type is always set */
    EventType event = static_cast<EventType>((se_type >> EVENT_TYPE_BIT_POS) & EVENT_TYPE_MASK);
    if (event != get_event_type()) {
        return false;
    }

    /* sensor type is always set */
    ipmi::command::generic::SensorType sensor = static_cast<ipmi::command::generic::SensorType>
        ((se_type >> SENSOR_TYPE_BIT_POS) & SENSOR_TYPE_MASK);
    if (sensor != get_sensor_type()) {
        return false;
    }

    /* either sensor number is given, or is undefined and not checked */
    SensorNumber sensor_num = static_cast<SensorNumber>((se_type >> SENSOR_NUMBER_BIT_POS) & EVENT_SENSOR_MASK);
    if ((sensor_num != UNDEFINED_SENSOR_NUMBER) && (sensor_num != get_sensor_number())) {
        return false;
    }

    /* either event specific offset is given, or undefined and not checked */
    EventOffset offset = static_cast<EventOffset>((se_type >> OFFSET_BIT_POS) & EVENT_DATA_OFFSET_MASK);
    if ((offset != UNDEFINED_OFFSET) && (offset != get_event_offset())) {
        return false;
    }

    EventData data = static_cast<EventData>((se_type >> DATA2_BIT_POS) & EVENT_DATA_VALUE_MASK);
    EventData mask = static_cast<EventData>((se_type >> DATA2_MASK_POS) & EVENT_DATA_PLAIN_VALUE_MASK);
    if (!event_data_match(static_cast<EventData>(data & (~mask)), get_event_data2())) {
        return false;
    }

    data = static_cast<EventData>((se_type >> DATA3_BIT_POS) & EVENT_DATA_VALUE_MASK);
    mask = static_cast<EventData>((se_type >> DATA3_MASK_POS) & EVENT_DATA_PLAIN_VALUE_MASK);
    if (!event_data_match(static_cast<EventData>(data & (~mask)), get_event_data3())) {
        return false;
    }

    return true;
}



std::string ipmi::SelRecordDiscrete::get_info() const {
    std::stringstream str{};
    str << "DISCRETE #" << std::hex << static_cast<unsigned>(get_type());
    return str.str();
}


std::string ipmi::SelRecordGenericOEM::get_info() const {
    std::stringstream str{};
    str << "OEM #" << std::hex << static_cast<unsigned>(get_type());
    return str.str();
}


std::string ipmi::SelRecordThreshold::get_info() const {
    return "THRESHOLD";
}


std::string ipmi::SelRecordSensor::get_info() const {
    return "SENSOR";
}
