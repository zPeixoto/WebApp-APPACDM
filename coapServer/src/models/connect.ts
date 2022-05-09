import mongoose from 'mongoose';

export const mongodbConnection = async () => {
  try{
    await mongoose.connect('mongodb://root:example@localhost:27017/');
    console.log('Connected to MongoDB');
  }catch(err){
    console.error(err);
  } 
}