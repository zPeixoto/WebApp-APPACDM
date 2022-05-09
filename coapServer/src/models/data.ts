import mongoose, { Schema, Document } from 'mongoose';


export interface IData extends Document {
    data: string;

}

const dataSchema = new Schema(
  {
    data: {
        type: String,
        required: true
    },
  },
  {
    timestamps: true,
    versionKey: false
  }
);


export default mongoose.model<IData>('DataCoap', dataSchema);

