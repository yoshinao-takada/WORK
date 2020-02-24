#include "base_c/BL_serialportconf.h"
#undef I
#include <rapidjson/document.h>

namespace {
    static const char* parity_labels[] = {
        "none", "even", "odd"
    };

    static const double stopbits_labels[] = {
        1.0, 1.5, 2.0
    };

    static const char* networkrole_labels[] = {
        "server", "client", "master", "slave", "repeater"
    };

    const char* BL_parity_format(BL_parity_t parity)
    {
        return (((int)parity) >= ARRAYSIZE(parity_labels)) ? nullptr : parity_labels[parity];
    }

    double BL_stopbits_tonumber(BL_stopbits_t stopbits)
    {
        return (((int)stopbits) >= ARRAYSIZE(stopbits_labels)) ? NAN : stopbits_labels[stopbits];
    }

    const char* BL_networkrole_format(BL_networkrole_t role)
    {
        return (((int)role) >= ARRAYSIZE(networkrole_labels)) ? nullptr : networkrole_labels[role];
    }

    int BL_parity_parse(const char* src_string, BL_ptr_t value)
    {
        int err = ENODATA;
        do {
            for (int i = 0; i < ARRAYSIZE(parity_labels); i++)
            {
                if (strcasecmp(parity_labels[i], src_string) == 0)
                {
                    *value._1i32 = i;
                    err = ESUCCESS;
                    break;
                }
            }
        } while (false);
        return err;
    }

    int BL_stopbits_parse(const char* src_string, BL_ptr_t value)
    {
        int err = ENODATA;
        *value._1i32 = (int)lround( 2.0 * (atof(src_string) - 1.0));
        if (*value._1i32 >= 0 && *value._1i32 < ARRAYSIZE(stopbits_labels))
        {
            err = ESUCCESS;
        }
        return err;
    }

    int BL_stopbits_parse(double src_number, BL_ptr_t value)
    {
        int err = ENODATA;
        int i_value = lround(2.0 * (src_number - 1.0));
        if (0 <= i_value && i_value < ARRAYSIZE(stopbits_labels))
        {
            err = ESUCCESS;
        }
        return err;
    }

    int BL_networkrole_parse(const char* src_string, BL_ptr_t value)
    {
        int err = ENODATA;
        do {
            for (int i = 0; i < ARRAYSIZE(networkrole_labels); i++)
            {
                if (strcasecmp(networkrole_labels[i], src_string) == 0)
                {
                    *value._1i32 = i;
                    err = ESUCCESS;
                    break;
                }
            }
        } while (false);
        return err;
    }
}

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

const char* BL_serialportconf_tojson(pcBL_serialportconf_and_name_t conf)
{
    rapidjson::Document doc;
    doc.SetObject();
    // construct doc from conf
    { 
        {
            rapidjson::Value v((int)(conf->base.address));
            doc.AddMember(BL_JSONTAG_ADDRESS, v, doc.GetAllocator());
        } {
            rapidjson::Value v(conf->base.baudrate);
            doc.AddMember(BL_JSONTAG_BAUDRATE, v, doc.GetAllocator());
        } {
            const char* value_string = BL_parity_format(conf->base.parity);
            rapidjson::Value v(value_string, strlen(value_string), doc.GetAllocator());
            doc.AddMember(BL_JSONTAG_PARITY, v, doc.GetAllocator());
        } {
            double value = BL_stopbits_tonumber(conf->base.stopbits);
            rapidjson::Value v(value);
            doc.AddMember(BL_JSONTAG_STOPBITS, v, doc.GetAllocator());
        } {
            const char* value_string = BL_networkrole_format(conf->base.role);
            rapidjson::Value v(value_string, doc.GetAllocator());
            doc.AddMember(BL_JSONTAG_ROLE, v, doc.GetAllocator());
        } {
            rapidjson::Value v(conf->name, doc.GetAllocator());
            doc.AddMember(BL_JSONTAG_DEVICENAME, v, doc.GetAllocator());
        }
    }

    // create a whole json string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* result_buffer = strdup(buffer.GetString());
    return result_buffer;
}

int BL_serialportconf_fromjson(const char* src_string, pBL_serialportconf_and_name_t* ppconf)
{
    int err = ESUCCESS;
    do {
        // convert a json string to DOM object, doc
        rapidjson::Document doc;
        doc.Parse(src_string);
        if (doc.HasParseError())
        {
            err = (int)doc.GetParseError();
            break;
        }

        // fill conf with the default values
        BL_serialportconf_t conf = BL_SERIALPORTCONF_DEFAULT;

        // extract conf members from doc
        {
            if (doc.HasMember(BL_JSONTAG_ADDRESS))
            {
                conf.address = (uint16_t)doc[BL_JSONTAG_ADDRESS].GetUint();
            }
            if (doc.HasMember(BL_JSONTAG_BAUDRATE))
            {
                conf.baudrate = doc[BL_JSONTAG_BAUDRATE].GetUint();
            }
            if (doc.HasMember(BL_JSONTAG_PARITY))
            {
                const char* parity = doc[BL_JSONTAG_PARITY].GetString();
                BL_ptr_t ptr_Parity = { (int8_t*)&(conf.parity) };
                if (ESUCCESS != (err = BL_parity_parse(parity, ptr_Parity)))
                {
                    break;
                }
            }
            if (doc.HasMember(BL_JSONTAG_ROLE))
            {
                const char* role = doc[BL_JSONTAG_ROLE].GetString();
                BL_ptr_t ptr_role = { (int8_t*)&(conf.role) };
                if (ESUCCESS != (err = BL_networkrole_parse(role, ptr_role)))
                {
                    break;
                }
            }
            if (doc.HasMember(BL_JSONTAG_STOPBITS))
            {
                double stopbits = doc[BL_JSONTAG_STOPBITS].GetDouble();
                BL_ptr_t ptr_stopbits = { (int8_t*)&(conf.stopbits) };
                if (ESUCCESS != (err = BL_stopbits_parse(stopbits, ptr_stopbits)))
                {
                    break;
                }
            }
            pBL_serialportconf_and_name_t  pconf = nullptr;
            if (doc.HasMember(BL_JSONTAG_DEVICENAME))
            {
                if (*ppconf)
                {
                    free((void*)(*ppconf));
                }
                const char* devicename = doc[BL_JSONTAG_DEVICENAME].GetString();
                size_t cb_name = strlen(devicename);
                size_t cb_alloc = sizeof(BL_serialportconf_and_name_t) + cb_name + 1;
                *ppconf = (pBL_serialportconf_and_name_t)calloc(cb_alloc, 1);
                pconf = *ppconf;
                strcpy(pconf->name, devicename);
            }
            BL_serialportconf_t& ref_conf = pconf->base;
            ref_conf.address = conf.address;
            ref_conf.baudrate = conf.baudrate;
            ref_conf.parity = conf.parity;
            ref_conf.role = conf.role;
            ref_conf.stopbits = conf.stopbits;
        }
    } while (false);
    return err;
}

pBL_serialportconf_and_name_t BL_serialportconf_and_name_new(uint32_t max_name_len)
{
    size_t cb_alloc = sizeof(BL_serialportconf_and_name_t) + max_name_len + 1;
    pBL_serialportconf_and_name_t p = (pBL_serialportconf_and_name_t)calloc(cb_alloc,1);
    if (!p) return p;
    BL_serialportconf_t defaults = BL_SERIALPORTCONF_DEFAULT;
    p->base.address = defaults.address;
    p->base.baudrate = defaults.baudrate;
    p->base.parity = defaults.parity;
    p->base.role = defaults.role;
    p->base.stopbits = defaults.stopbits;
    return p;
}

pBL_serialportconf_and_name_t BL_serialportconf_and_name_new2(const char* initial_name)
{
    pBL_serialportconf_and_name_t p = BL_serialportconf_and_name_new(strlen(initial_name));
    if (!p) return p;
    strcpy(p->name, initial_name);
    return p;
}