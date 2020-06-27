#include    "base_g/BL_twolines.h"

int BL_twolines_nearest(pcBL_twolines_t conf, BL_3r32_t PTa, BL_3r32_t PTb)
{
    int err = ESUCCESS;
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_6r32_t mv;
    BL_2r32_t t;
    BL_1r32_t DaDa, DaDb, DbDb, PaDa, PbDb, PaDb, PbDa;
    fn->mul(conf->Da, conf->Da, &DaDa, 1, 3, 1);
    fn->mul(conf->Da, conf->Db, &DaDb, 1, 3, 1);
    fn->mul(conf->Db, conf->Db, &DbDb, 1, 3, 1);
    fn->mul(conf->Pa, conf->Da, &PaDa, 1, 3, 1);
    fn->mul(conf->Pa, conf->Db, &PaDb, 1, 3, 1);
    fn->mul(conf->Pb, conf->Da, &PbDa, 1, 3, 1);
    fn->mul(conf->Pb, conf->Db, &PbDb, 1, 3, 1);
    mv[0] = DaDa;
    mv[1] = -DaDb;
    mv[2] = PbDa - PaDa;
    mv[3] = mv[1];
    mv[4] = DbDb;
    mv[5] = PaDb - PbDb;
    if (ESUCCESS == (err = fn->soldense(mv, 3, 2, t)))
    {
        PTa[0] = conf->Pa[0] + t[0] * conf->Da[0];
        PTa[1] = conf->Pa[1] + t[0] * conf->Da[1];
        PTa[2] = conf->Pa[2] + t[0] * conf->Da[2];
        PTb[0] = conf->Pb[0] + t[1] * conf->Db[0];
        PTb[1] = conf->Pb[1] + t[1] * conf->Db[1];
        PTb[2] = conf->Pb[2] + t[1] * conf->Db[2];
    }
    return err;
}